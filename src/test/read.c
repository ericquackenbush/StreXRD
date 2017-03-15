#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <math.h>
#include <ctype.h>
#include <string.h>
#include <time.h>

#define BYTE char
#define WORD short int

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
#define PACKIDENTIFIER "\nCCP4 packed image, X: %04d, Y: %04d\n"
#define sizeofa(array) sizeof array / sizeof array[ 0 ]

void unpack_word(unsigned char *cimg, int x, int y, WORD *img)
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

void get_pck(int s, unsigned char *cimg, WORD *img)
{
	int x = 0, y = 0, i = 0, j=0, c = 0;
	char header[BUFSIZ];
	unsigned char	*ci;

		header[0] = '\n';
		header[1] = 0;

		ci = cimg;
		/*
		 * Scan file until PCK header is found
		 */

		// get_pck( i, cimage, (signed short *)i2_image);

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

		unpack_word(ci, x, y, img);
}

void swapint32(unsigned char *data, int nbytes)
{
	int i;
	unsigned char t1, t2, t3, t4;

    for(i=nbytes/4;i--;) {
        t1 = data[i*4+3];
        t2 = data[i*4+2];
        t3 = data[i*4+1];
        t4 = data[i*4+0];
        data[i*4+0] = t1;
        data[i*4+1] = t2;
        data[i*4+2] = t3;
        data[i*4+3] = t4;
    }
}


int main()
{
	int	*i4, *i4_image;
	unsigned short 	*i2, *i2_image;
	int nx;
	int ny;
	int no;
	int i;
	int j;
	int iadd;
	int	head[3];
	int byteswap = 0;
	unsigned char 	*cimage;

	FILE *fp;
	fp  = fopen("image.mar3450",  "r+" );

	fread( head, sizeof( int ), 3, fp);

	if ( head[0] != 1234 ) {
		byteswap = 1;
		swapint32( (unsigned char *)head, 3*sizeof(int) );
		if ( head[0] == 1234 ) {
			printf("ERROR: First integer must be 1234 but is %d\n", head[0]);
			exit(-1);
		}
	}

	nx = ny = head[1];
	no = head[2];

	i4_image = (int *)malloc( nx*ny*sizeof(int));
	memset( (char *)i4_image, 0, sizeof(int)*nx*ny );
	
	// Read entire image into memory
	i = fseek( fp, 0,   SEEK_END );
	i = ftell( fp );
	cimage = (unsigned char *)malloc( i*sizeof(char));
	rewind( fp );
	j = fread( cimage, sizeof(char), i, fp);
	printf("%d bytes read from %s\n",j, "test.mar3450");

	// i2_image begins 3450*3450/2 * 4 bytes later than i4_image
	i2_image= (unsigned short *)(i4_image+nx*ny/2);

	get_pck( i, cimage, (signed short *)i2_image);

	// Release memeory for cimage
	free( cimage );

	/* Transform i2 array into i4 array */
	i4 = i4_image;
	i2 = i2_image;
	for ( i=0; i<nx*ny; i++, i2++, i4++ ) {
		*i4 = (int)*i2;
	}

	/* There are some high intensity values stored in the image */
	if ( no ) {
		/* Go to start of overflow record */
		i = fseek( fp, 4096,   SEEK_SET );
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
		for(i=0;i<no; i++ ) {
			j = fread( head, sizeof(int), 2, fp);

			if ( byteswap)
				swapint32( (unsigned char *)head, sizeof(int)*2 );

			iadd = head[0];
			if ( iadd >= (nx*ny) || iadd < 0 ) continue;

			i4_image[iadd] = head[1];
		}
	}

	/* Close input file */
	fclose( fp );

	fp  = fopen("test.img", "w+" );	
	fwrite( i4_image, sizeof(int), nx*ny, fp );
	fclose( fp );

	printf("i4_image[8340138]: %d\n", i4_image[8340138]);

	return 0;
}