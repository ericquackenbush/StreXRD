/**********************************************************************
 *
 * pck:		Converts a packed byte array into unsigned short array
 *
 * Copyright by:	Dr. Claudio Klein, marXperts GmbH
 * Original code by:	Dr. Jan Pieter Abrahams, Universiteit Leiden
 * Matlab Interface (mexFunction):	Anders Filsøe Pedersen,
 *					Technical University of Denmark, Lyngby
 *
 * Version: 	1.0
 * Date:	26/08/2014
 *
 **********************************************************************/
#include <stdio.h>
#include <stddef.h>
#include <math.h>
#include <ctype.h>
#include <string.h>
#include <matrix.h>
#include <mex.h>

#define BYTE char
#define WORD short int
#define LONG int

#define PACKIDENTIFIER "\nCCP4 packed image, X: %04d, Y: %04d\n"
#define PACKBUFSIZ BUFSIZ
#define DIFFBUFSIZ 16384L
#define max(x, y) (((x) > (y)) ? (x) : (y))
#define min(x, y) (((x) < (y)) ? (x) : (y))
#define abs(x) (((x) < 0) ? (-(x)) : (x))
const LONG setbits[33] = {0x00000000L, 0x00000001L, 0x00000003L, 0x00000007L,
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

/*
 * Function prototypes
 */
void 		get_pck 	(int, unsigned char *,  WORD *);
static void 	unpack_word	(unsigned char *, int, int, WORD *);

/***************************************************************************
 * Function: get_pck
 *	     s is the total size in bytes of cimg
 *	     cimg is a uchar array containing the entire file
 *	     img is a 16-bit integer array with x*y pixels
 ***************************************************************************/
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

/*****************************************************************************
 * Function: unpack_word
 * Unpacks a packed image into the WORD-array 'img'.
 *****************************************************************************/
static void unpack_word(unsigned char *cimg, int x, int y, WORD *img)
{
int 		valids = 0, spillbits = 0, usedbits, total = x * y;
LONG 		window = 0L, spill, pixel = 0, nextint, bitnum, pixnum;
static int 	bitdecode[8] = {0, 4, 5, 6, 7, 8, 16, 32};

	while (pixel < total) {
	    if (valids < 6) {
    		if (spillbits > 0) {
      			window |= shift_left(spill, valids);
        		valids += spillbits;
        		spillbits = 0;
			}
  	    	else {
			/* spill gets set to the value where cimg points to, then increments pointer
  	    	to next memory address */
  			spill = (LONG) *cimg++;
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
	          		    spill = (LONG) *cimg++;
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

/*******************************************************************
 *  Function: mexFunction
 *  This is the gateway function to MatLab.
 *
 *  Use: img = unpack345(cimg,sd)
 *  img is the uncompressed image
 *  cimg is the raw uncompressed data
 *  sd is the size of the image, e.g. 3450 or 2300 pixels
 *
 *  Compile using the mex command
 *
 ******************************************************************/
void mexFunction( int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    size_t s;					/* Compressed file length */
	int sd;						/* Side length of uncompressed image */
	unsigned char *cimg;					/* Compressed image */
    unsigned short *outImage;	/* Output image */

	/* Get the length of the compressed data */
	s = mxGetNumberOfElements(prhs[0]);
    
	/* Get the compressed data (cimg) */
	cimg = mxGetChars(prhs[0]);
    
	/* Get the side length of the uncompressed image (sd) */
	sd = mxGetScalar(prhs[1]);
        
    /* create the output matrix (img) */
    plhs[0] = mxCreateNumericMatrix((mwSize)sd,(mwSize)sd,mxUINT16_CLASS,mxREAL);

    /* get a pointer to the real data in the output matrix (img) */
    outImage = mxGetData(plhs[0]);

    /* call the computational routine */
    get_pck(s,cimg,outImage);
    
}