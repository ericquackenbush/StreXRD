/* Copyright 2017 Eric Quackenbush */

#define _GLIBCXX_USE_CXX11_ABI 0
#include <iostream>
#include <fstream>
#include <iterator>
#include <boost/algorithm/string.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <read_mar.h>

typedef unsigned char BYTE;
typedef short int WORD;

#define PACKIDENTIFIER "\nCCP4 packed image, X: %04d, Y: %04d\n"
const long setbits[33] = {0x00000000L, 0x00000001L, 0x00000003L, 0x00000007L,
			  0x0000000FL, 0x0000001FL, 0x0000003FL, 0x0000007FL,
			  0x000000FFL, 0x000001FFL, 0x000003FFL, 0x000007FFL,
			  0x00000FFFL, 0x00001FFFL, 0x00003FFFL, 0x00007FFFL,
			  0x0000FFFFL, 0x0001FFFFL, 0x0003FFFFL, 0x0007FFFFL,
			  0x000FFFFFL, 0x001FFFFFL, 0x003FFFFFL, 0x007FFFFFL,
			  0x00FFFFFFL, 0x01FFFFFFL, 0x03FFFFFFL, 0x07FFFFFFL,
			  0x0FFFFFFFL, 0x1FFFFFFFL, 0x3FFFFFFFL, 0x7FFFFFFFL,
                          0xFFFFFFFFL};
#define shift_left(x, n)  (((x) & setbits[32 - (n)]) << (n))
#define shift_right(x, n) (((x) >> (n)) & setbits[32 - (n)])

ReadMar::ReadMar(std::string filename)
{
	// open the file
    std::ifstream file(filename, std::ios::binary);

    // disable skipping of leading whitespace
    file.unsetf(std::ios::skipws);

    // get size of file
    file.seekg(0, std::ios::end);
    fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    // allocate memory
    fileBuf.reserve(fileSize);

    // read data into vector
    fileBuf.insert(fileBuf.begin(),
               std::istream_iterator<BYTE>(file),
               std::istream_iterator<BYTE>());
}

std::vector<int> ReadMar::byte_to_int(std::vector<BYTE> b)
{
	std::vector<int> a {-1};
	if (b.size() %  4 == 0)
	{
		int n = 0;
		for (int i = 0; i < b.size(); i+=4)
		{
			a[n] = b[i] + b[i + 1] * 256 + b[i + 2] * 256 * 256 + 
				b[i + 3] * 256 * 256 * 256;
			n++;
		}
	}
	else
	{
		std::cout << "Invalid size!" << "\n";
	}
	return a;
}

void ReadMar::byte_swap(std::vector<BYTE> b)
{
	int i;
	BYTE t1, t2, t3, t4;
	for (i = b.size()/4; i--;)
	{
		t1 = b[i * 4 + 3];
		t2 = b[i * 4 + 2];
		t3 = b[i * 4 + 1];
		t4 = b[i * 4 + 0];
		b[i * 4 + 0] = t1;
		b[i * 4 + 1] = t2;
		b[i * 4 + 2] = t3;
		b[i * 4 + 3] = t4;
	}
}

int ReadMar::get_file_size()
{
	return (int)fileSize;
}

void ReadMar::get_header()
{
	// get first 4096 bytes
	std::vector<BYTE>::const_iterator first = fileBuf.begin();
	std::vector<BYTE>::const_iterator last = fileBuf.begin() + 4096;
	std::vector<BYTE> header(first, last);

	std::vector<BYTE> text(3968);
	for (int i = 128; i < 4096; i++) {
		text[i - 128] = header[i];
	}

	std::string str = " ";

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
		std::string str(chars);
		if (str.find("END") == 0)
			break;
		list.push_back(str);
	}

	list.erase(list.begin());
}

void ReadMar::print_header(std::string filename)
{
	if (!list.empty())
	{
		// print header to text file
		std::ofstream myfile(filename);
		if (myfile.is_open())
		{
			for (int count = 0; count < list.size(); count++)
			{
				myfile << list[count] << std::endl;
			}
			myfile.close();
			std::cout << "File written successfully" << "\n";
		}
		else std::cout << "Unable to open file" << "\n";
	}
	else
	{
		std::cout << "Could not print header file. Use get_header() first!" << "\n";
	}
}


void ReadMar::unpack_word(unsigned char *cimg, int x, int y, WORD *img)
{
	int 		valids = 0, spillbits = 0, usedbits, total = x * y;
	long 		window = 0L, spill, pixel = 0, nextint, bitnum, pixnum;
	static int 	bitdecode[8] = {0, 4, 5, 6, 7, 8, 16, 32};

	while (pixel < total) {
	    if (valids < 6) {
    		if (spillbits > 0) {
      			window |= shift_left(spill, valids);
        		valids += spillbits;
        		spillbits = 0;
			}
  	    	else {
	  			spill = (int) *cimg++;
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
	          		    spill = (int) *cimg++;
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
	     		    	img[pixel] = (WORD) (nextint +
	                              (img[pixel-1] + img[pixel-x+1] +
	                               img[pixel-x] + img[pixel-x-1] + 2) / 4);
	        		    ++pixel;
					}
	      			else if (pixel != 0) {
	          		    img[pixel] = (WORD) (img[pixel - 1] + nextint);
	            		    ++pixel;
					}
	      			else
	        		    img[pixel++] = (WORD) nextint;
		    	}
			}
    	}
	}
}

void ReadMar::get_pck(int s, unsigned char *cimg, WORD *img)
{
	int x = 0, y = 0, i = 0, j=0, c = 0;
	char header[BUFSIZ];
	unsigned char	*ci;

		header[0] = '\n';
		header[1] = 0;

		ci = cimg;

		while ((x == 0) || (y == 0)) {
			c = i = x = y = 0;
			while ((++i < BUFSIZ) && (c != '\n') && (x==0) && (y==0)) {
				if ((header[i] = c = *ci++ ) == '\n')
					sscanf(header, PACKIDENTIFIER, &x, &y);
				j++;
				if ( j >= s ) break;
			}
			if ( j >= s ) break;
		}

		std::cout << "Total number of pixels: " << x*y << "\n";

		unpack_word(ci, x, y, img);
}

std::vector<int> ReadMar::read_file()
{
	int *i4, *i4_image;
    unsigned short  *i2, *i2_image;
	int nx, ny, no;
	int byteSwap = 0;
	unsigned char   *cimage;

	// get first 3 integers
	std::vector<BYTE>::const_iterator first = fileBuf.begin();
	std::vector<BYTE>::const_iterator last = fileBuf.begin() + 12;
	std::vector<BYTE> b(first, last);

	std::vector<int> head = byte_to_int(b);

	// check initial value
	if (head[0] != 1234)
	{
		// check for big endian encoding
		byte_swap(b);
		head = byte_to_int(b);
		if (head[0] = 1234)
		{
			byteSwap = 1;
		}
		else
		{
			printf("ERROR: First integer must be 1234 but is %d\n", head[0]);
			exit(1);
		}
	}

	nx = ny = head[1];
	no = head[2];

	printf("Image size: %d x %d\n", nx, ny);
	printf("Number of high intensity pixels: %d\n", no);

	// allocate memory
    std::vector<int> output;
    output.resize(nx*ny);

    i2_image= (unsigned short *)(&output[0]+nx*ny/2);
    cimage = &fileBuf[0];

    get_pck( fileSize, cimage, (signed short *)i2_image);
	free( cimage );

    // transform i2 array into i4 array
    i2 = i2_image;
    for (int i = 0; i < nx*ny; i++, i2++) {
        output[i] = (int)*i2;
    }

	return output;
}

int main()
{
	std::cout << "A program for reading .mar3450 files and returning an integer vector.\n";
	std::string filename = "/home/eric/StreXRD/src/image.mar3450";
	ReadMar new_file(filename);
	int fileSize = new_file.get_file_size();
	std::cout << "Size of file: " << fileSize << " bytes\n";

	std::vector<int> output = new_file.read_file();

	std::cout << "Size: " << output.size() << "\n";

	cv::Mat m = cv::Mat(3450, 3450, CV_32S);
	memcpy(m.data, output.data(), output.size() * sizeof(int));
	cv::Mat inputImage;
	m.convertTo(inputImage, CV_8UC1);

	cv::resize(inputImage, inputImage, cv::Size(inputImage.cols/6, inputImage.rows/6));
	cv::namedWindow( "Display window", cv::WINDOW_AUTOSIZE );
	cv::imshow( "Display window", inputImage );
	cv::waitKey(0);

	return 0;
}