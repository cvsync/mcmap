/*
 *  extractcolors.h
 *  MCMap Live
 *
 *  Created by DK on 10/18/10.
 *  Modified for mcmap by Zahl
 *
 */

#include "extractcolors.h"
#include "colors.h"

// This beast of an array maps block IDs to tile locations in terrain.png
// The tile x and tile y are 0-index. A value of -1,-1 means no tile exists
// Extra alpha multiplier is there for textures that are shared and might need to
// be lighter for one use than another.
//                                  { tile x, tile y, extra alpha multiplier)
static const int special_sauce[92][3] = {
	{	-1,	-1,	10	},
	{	 1,	 0,	10	},
	{	 0,	 0,	10	},
	{	 2,	 0,	10	},
	{	 0,	 1,	10	},
	{	 4,	 0,	10	},
	{	15,	 0,	10	},
	{	 1,	 1,	10	},
	{	15,	12,	 5	},
	{	15,	12,	 5	},
	{	15,	14,	10	},
	{	15,	14,	10	},
	{	 2,	 1,	10	},
	{	 3,	 1,	10	},
	{	 0,	 2,	10	},
	{	 1,	 2,	10	},
	{	 2,	 2,	10	},
	{	 5,	 1,	 7	},
	{	 4,	 3,	10	},
	{	 0,	 3,	10	},
	{	 1,	 3,	 5	},
	{	-1,	-1,	10	},
	{	-1,	-1,	10	},
	{	-1,	-1,	10	},
	{	-1,	-1,	10	},
	{	-1,	-1,	10	},
	{	-1,	-1,	10	},
	{	-1,	-1,	10	},
	{	-1,	-1,	10	},
	{	-1,	-1,	10	},
	{	-1,	-1,	10	},
	{	-1,	-1,	10	},
	{	-1,	-1,	10	},
	{	-1,	-1,	10	},
	{	-1,	-1,	10	},
	{	 0,	 4,	10	},
	{	-1,	-1,	10	},
	{	13,	 0,	10	},
	{	12,	 0,	10	},
	{	13,	 1,	10	},
	{	12,	 1,	10	},
	{	 7,	 2,	10	},
	{	 6,	 2,	10	},
	{	 5,	 0,	10	},
	{	 5,	 0,	10	},
	{	 7,	 0,	10	},
	{	 8,	 0,	10	},
	{	 3,	 2,	10	},
	{	 4,	 2,	10	},
	{	 5,	 2,	10	},
	{	 0,	 5,	30	},
	{	15,	15,	 3	},
	{	 1,	 4,	10	},
	{	 4,	 0,	10	},
	{	11,	 1,	10	},
	{	 4,	 6,	10	},
	{	 2,	 3,	10	},
	{	 8,	 2,	10	},
	{	12,	 3,	10	},
	{	15,	 5,	10	},
	{	 7,	 5,	10	},
	{	12,	 2,	10	},
	{	13,	 3,	10	},
	{	 0,	 0,	10	},
	{	 1,	 6,	10	},
	{	 3,	 5,	10	},
	{	 0,	 8,	10	},
	{	 0,	 1,	10	},
	{	 4,	 0,	10	},
	{	 3,	 6,	10	},
	{	 0,	 6,	10	},
	{	 2,	 6,	10	},
	{	 4,	 0,	10	},
	{	 3,	 3,	10	},
	{	 3,	 3,	10	},
	{	 3,	 7,	10	},
	{	 3,	 6,	10	},
	{	 2,	 6,	 1	},
	{	 2,	 4,	10	},
	{	 3,	 4,	10	},
	{	 2,	 4,	10	},
	{	 6,	 4,	10	},
	{	 8,	 4,	10	},
	{	 9,	 4,	10	},
	{	10,	 4,	10	},
	{	 5,	 1,	 9	},
	{	 6,	 7,	10	},
	{	 7,	 6,	10	},
	{	 8,	 6,	10	},
	{	 9,	 6,	10	},
	{	 14,	 0,	 5	},
	{	 8,	 7,	10	}
};

bool getTileRGBA(uint8_t *textures, int tilesize, int sauce_index, int &r, int &g, int &b, int &a, int &noise)
{
	r = 0;
	g = 0;
	b = 0;
	a = 0;
	noise = 0;

	const int x = special_sauce[sauce_index][0];
	const int y = special_sauce[sauce_index][1];

	if (x == -1) {
		return false;
	}

	int n = tilesize * tilesize;
	int bytesperrow = 16 * tilesize * 4;

	int sx = x * tilesize * 4;
	int sy = y * tilesize;

	for (int j = sy; j < (sy + tilesize); j++) {
		for (int i = sx; i < (sx + tilesize * 4); i = i + 4) {
			// If the pixel is entirely transparent
			if (textures[i+3+j* (bytesperrow) ] == 0) {
				n--;
			} else {
				r = r + textures[i+j* (bytesperrow) ];
				g = g + textures[i+1+j* (bytesperrow) ];
				b = b + textures[i+2+j* (bytesperrow) ];
				a = a + textures[i+3+j* (bytesperrow) ];
			}

		}
	}

	double var = 0;

	if (n == 0) {
		return false;
	}
	r = r / n;
	g = g / n;
	b = b / n;
	a = a / n; // Changed my mind again
	a = int(float(a) * (float(special_sauce[sauce_index][2]) / 10.0f));

	for (int j = sy; j < (sy + tilesize); j++) {
		for (int i = sx; i < (sx + tilesize * 4); i = i + 4) {
			// If the pixel is not entirely transparent
			if (textures[i+3+j* (bytesperrow) ] != 0) {
				var = var + (pow(textures[i+j * (bytesperrow)] - r, 2.0) + pow(textures[i+1+j * (bytesperrow)] - b, 2.0) + pow(textures[i+2+j * (bytesperrow)] - g, 2.0)) / (3.0 * n);
			}
		}
	}

	noise = int (8 * var / ( (n * n - 1) / 12));
	if (noise > 255) {
		noise = 255;
	}
	if (a > 255) {
		a = 255;
	}
	return true;
}