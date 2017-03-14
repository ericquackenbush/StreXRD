#include <stdio.h>
#include <algorithm>
#include <ctype.h>
#include <vector>
#include <iostream>
#include <string>
#include <fstream>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <boost/algorithm/string.hpp>

using namespace std;
using namespace cv;

#define PACKIDENTIFIER "\nCCP4 packed image, X: %04d, Y: %04d\n"
const int setbits[33] = { 0x00000000L, 0x00000001L, 0x00000003L, 0x00000007L,
0x0000000FL, 0x0000001FL, 0x0000003FL, 0x0000007FL,
0x000000FFL, 0x000001FFL, 0x000003FFL, 0x000007FFL,
0x00000FFFL, 0x00001FFFL, 0x00003FFFL, 0x00007FFFL,
0x0000FFFFL, 0x0001FFFFL, 0x0003FFFFL, 0x0007FFFFL,
0x000FFFFFL, 0x001FFFFFL, 0x003FFFFFL, 0x007FFFFFL,
0x00FFFFFFL, 0x01FFFFFFL, 0x03FFFFFFL, 0x07FFFFFFL,
0x0FFFFFFFL, 0x1FFFFFFFL, 0x3FFFFFFFL, 0x7FFFFFFFL,
0xFFFFFFFFL };
#define shift_left(x, n)  (((x) & setbits[32 - (n)]) << (n))
#define shift_right(x, n) (((x) >> (n)) & setbits[32 - (n)])
typedef unsigned char BYTE;

// header variables
int marker;
int size;
int num_high_intensity_pixels;

char *filename;

// get the size of a file
long getFileSize(FILE *file)
{
	long lCurPos, lEndPos;
	lCurPos = ftell(file);
	fseek(file, 0, 2);
	lEndPos = ftell(file);
	fseek(file, lCurPos, 0);
	return lEndPos;
}

int byteToInt(BYTE b1, BYTE b2, BYTE b3, BYTE b4)
{
	int a;
	a = b1 + b2 * 256 + b3 * 256 * 256 + b4 * 256 * 256 * 256;
	return a;
}

void printHeader(BYTE header[4096])
{
	char text[3968];
	for (int i = 128; i < 4096; i++) {
		text[i - 128] = header[i];
	}

	vector<string> list;
	string str = " ";

	char chars[64];
	for (int i = 0; i < 3968; i += 64) {
		for (int j = 0; j < 64; j++) {
			if (text[i + j] >= 32 && text[i + j] <= 126) {
				chars[j] = text[i + j];
			}
			else {
				chars[j] = '\0';
			}
		}
		string str(chars);
		if (str.find("END") == 0)
			break;
		list.push_back(str);
	}

	list.erase(list.begin());

	// print header to text file
	string fn(filename);
	fn.append(".txt");
	char *fileName = (char*)fn.c_str();
	ofstream myfile(fileName);
	if (myfile.is_open())
	{
		for (int count = 0; count < list.size(); count++) {
			myfile << list[count] << endl;
		}
		myfile.close();
		cout << "File written successfully" << "\n";
	}
	else cout << "Unable to open file" << "\n";
}

void unpack(int s, unsigned char *cimg, short int *img)
{
	int x = 0, y = 0, i = 0, j = 0, c = 0;
	int valids = 0, spillbits = 0, usedbits, total;
	int window = 0L, spill, pixel = 0, nextint, bitnum, pixnum;
	static int  bitdecode[8] = { 0, 4, 5, 6, 7, 8, 16, 32 };
	char header[BUFSIZ];
	unsigned char   *ci;

	ci = cimg;

	header[0] = '\n';
	header[1] = 0;

	while ((x == 0) || (y == 0)) {
		c = i = x = y = 0;
		while ((++i < BUFSIZ) && (c != '\n') && (x == 0) && (y == 0)) {
			if ((header[i] = c = *cimg++) == '\n')
				sscanf(header, PACKIDENTIFIER, &x, &y);
			j++;
			if (j >= s) break;
		}
		if (j >= s) break;
	}

	total = x * y;

	while (pixel < total) {
		if (valids < 6) {
			if (spillbits > 0) {
				window |= shift_left(spill, valids);
				valids += spillbits;
				spillbits = 0;
			}
			else {
				spill = (int)*cimg++;
				spillbits = 8;
			}
		}
		else {
			pixnum = 1 << (window & setbits[3]);
			window = shift_right(window, 3);
			bitnum = bitdecode[window & setbits[3]];
			window = shift_right(window, 3);
			valids -= 6;
			while ((pixnum > 0) && (pixel < total)) {
				if (valids < bitnum) {
					if (spillbits > 0) {
						window |= shift_left(spill, valids);
						if ((32 - valids) > spillbits) {
							valids += spillbits;
							spillbits = 0;
						}
						else {
							usedbits = 32 - valids;
							spill = shift_right(spill, usedbits);
							spillbits -= usedbits;
							valids = 32;
						}
					}
					else {
						spill = (int)*cimg++;
						spillbits = 8;
					}
				}
				else {
					--pixnum;
					if (bitnum == 0)
						nextint = 0;
					else {
						nextint = window & setbits[bitnum];
						valids -= bitnum;
						window = shift_right(window, bitnum);
						if ((nextint & (1 << (bitnum - 1))) != 0)
							nextint |= ~setbits[bitnum];
					}
					if (pixel > x) {
						img[pixel] = (short int)(nextint +
							(img[pixel - 1] + img[pixel - x + 1] +
								img[pixel - x] + img[pixel - x - 1] + 2) / 4);
						++pixel;
					}
					else if (pixel != 0) {
						img[pixel] = (short int)(img[pixel - 1] + nextint);
						++pixel;
					}
					else
						img[pixel++] = (short int)nextint;
				}
			}
		}
	}
}

void swapint32(unsigned char *data, int nbytes)
{
	int i;
	unsigned char t1, t2, t3, t4;

	for (i = nbytes / 4; i--;) {
		t1 = data[i * 4 + 3];
		t2 = data[i * 4 + 2];
		t3 = data[i * 4 + 1];
		t4 = data[i * 4 + 0];
		data[i * 4 + 0] = t1;
		data[i * 4 + 1] = t2;
		data[i * 4 + 2] = t3;
		data[i * 4 + 3] = t4;
	}
}


int readmar(int argc, char* argv[])
{
	filename = argv[1];
	const char *filePath = argv[1];
	int *i4, *i4_image;
	unsigned short  *i2, *i2_image;
	int nx;
	int ny;
	int no;
	int i;
	int j;
	int iadd;
	int head[3];
	int byteswap = 0;
	unsigned char   *cimage;
	BYTE header[4096];

	FILE *fp;
	fp = fopen(filename, "r+");

	fread(head, sizeof(int), 3, fp);

	if (head[0] != 1234) {
		byteswap = 1;
		swapint32((unsigned char *)head, 3 * sizeof(int));
		if (head[0] == 1234) {
			printf("ERROR: First integer must be 1234 but is %d\n", head[0]);
			exit(-1);
		}
	}

	nx = ny = head[1];
	no = head[2];

	i4_image = (int *)malloc(nx*ny * sizeof(int));
	memset((char *)i4_image, 0, sizeof(int)*nx*ny);

	printf("%lu\n", sizeof(int));

	// Read entire image into memory
	i = fseek(fp, 0, SEEK_END);
	i = ftell(fp);
	cimage = (unsigned char *)malloc(i * sizeof(char));
	rewind(fp);
	j = fread(cimage, sizeof(char), i, fp);
	printf("%d bytes read from %s\n", j, filename);

	// get header data
	for (int i = 0; i < 4096; i++) {
		header[i] = cimage[i];
	}

	i2_image = (unsigned short *)(i4_image + nx*ny / 2);

	// extract the header data
	// printHeader(header);

	// unpack raw data
	unpack(i, cimage, (signed short *)i2_image);

	// Release memeory for cimage
	free(cimage);

	// Transform i2 array into i4 array
	i4 = i4_image;
	i2 = i2_image;
	for (i = 0; i<nx*ny; i++, i2++, i4++) {
		*i4 = (int)*i2;
	}

	// There are some high intensity values stored in the image */
	if (no) {
		/* Go to start of overflow record */
		i = fseek(fp, 4096, SEEK_SET);
		/*
		* Read overflows. Read a pair of values ( strong ), where
		* first  value = address of pixel in i4_image array
		* second value = intensity of pixel (32-bit)
		* In order to save memory, we don't allocate a 32-bit array
		* for all data, but a 16-bit array and an 8-bit array.
		* The final intensity 24-bit intensity is stored as:
		* 24-bit-value = 16-bit-value + (8-bit-value)*65535;
		* Note: the maximum intensity the scanner may produce is 250.000
		*       Beyond the saturation of the ADC, all saturated pixels
		*       get an intensity of 999.999 !
		*/
		for (i = 0; i<no; i++) {
			j = fread(head, sizeof(int), 2, fp);

			if (byteswap)
				swapint32((unsigned char *)head, sizeof(int) * 2);

			iadd = head[0];
			if (iadd >= (nx*ny) || iadd < 0) continue;

			i4_image[iadd] = head[1];
		}
	}

	// Close input file
	fclose(fp);

	Mat M1(3450, 3450, CV_16UC4, i4_image); // For RGB image
	Mat M2;
	cvtColor(M1, M2, CV_BGRA2GRAY);

	namedWindow("Display window", WINDOW_NORMAL);
	imshow("Display window", M2);

	waitKey(0);
	return 0;
}