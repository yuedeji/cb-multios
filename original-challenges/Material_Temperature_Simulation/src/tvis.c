/*

Author: Joe Rogers <joe@cromulence.co>

Copyright (c) 2014 Cromulence LLC

Permission is hereby granted, cgc_free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

*/
#include <libcgc.h>
#include "stdlib.h"
#include "stdint.h"
#include "room.h"
#include "malloc.h"

extern double *TGrid;
extern double *HGrid;
#define pGRID(grid,x,y,z) (grid + x + y*X + z*X*Y)
extern cgc_uint32_t X;
extern cgc_uint32_t Y;
extern cgc_uint32_t Z;

#define CLEAR_SCREEN cgc_printf("\033[2J\033[H")

#define MAX_WIDTH 10.0
#define MAX_HEIGHT 10.0

// function to use cb's lib functions for float string formatting
void cgc_float_to_str( double, char *);
double output[10][10];
void cgc_output_str(cgc_uint32_t x, cgc_uint32_t y, char *buf) {
	cgc_float_to_str(output[x][y]-273.15, buf);
}

double cgc_TGridAverage(cgc_uint32_t out_x, cgc_uint32_t out_y, cgc_uint32_t startx, cgc_uint32_t starty, cgc_uint32_t countx, cgc_uint32_t county, cgc_uint32_t z) {
	double sum = 0.0;
	//cgc_uint32_t count = 0;
	double count = 0.0;
	cgc_uint32_t x, y;
	char buf[64];


	// don't go past the end of the TGrid array
	countx = (startx + countx > X) ? (X - startx) : countx;
	county = (starty + county > Y) ? (Y - starty) : county;

	for (x = startx; x < startx+countx; x++) {
		for (y = starty; y < starty+county; y++) {
			sum += *pGRID(TGrid,x,y,z);
			count++;
		}
	}

	output[out_x][out_y] = sum/count;

	return (sum/count);

}

cgc_int32_t cgc_GraphTemps(double *grid) {
	cgc_uint32_t x, y, z;
	cgc_uint32_t out_x, out_y;
	cgc_uint32_t startx, starty;
	cgc_uint32_t countx, county;

	CLEAR_SCREEN;

	countx = (X/MAX_WIDTH)+1;
	county = (Y/MAX_HEIGHT)+1;

	for (z = 0; z < Z; z++) {
		cgc_printf("z: @d\n", z);

		starty = 0;
		y = 0;
		while (starty < Y) {
			startx = 0;
			x = 0;

			while (startx < X) {
				// average of subarray (startx,starty) to (startx+countx,starty+county)
				// converted back to celcius
				cgc_TGridAverage(x, y, startx,starty,countx,county,z);
	
				startx += countx;
				x++;
			}
			starty += county;
			y++;
		}

		for (out_x = 0; out_x < x; out_x++) {
			for (out_y = 0; out_y < y; out_y++) {

				double datapoint;
				datapoint = output[out_x][out_y] - 273.15;
	
				// print out decimal point aligned
				// the cgc_printf @f function rounds up or down by
				// by using .5*F32_PRECISION if the value is positive
				// or negative.  So, we need to do the same to 
				// make sure the resulting string is aligned properly
				double round = 0.0;
				if (datapoint < 0.0) {
					round = -0.000005;
				} else if (datapoint > 0.0) {
					round = 0.000005;
				}
				if (datapoint+round <= -100.0) {
					cgc_printf("@f ", datapoint);
				} else if (datapoint+round <= -10.0) {
					cgc_printf(" @f ", datapoint);
				} else if (datapoint+round < 0.0) {
					cgc_printf("  @f ", datapoint);
				} else if (datapoint+round < 10.0) {
					cgc_printf("   @f ", datapoint);
				} else if (datapoint+round < 100.0) {
					cgc_printf("  @f ", datapoint);
				} else {
					cgc_printf(" @f ", datapoint);
				}
			}
			cgc_printf("\n");
		}
		cgc_printf("\n");
	}

	return(0);

}
