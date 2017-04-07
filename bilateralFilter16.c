#include <stdio.h>
#include <stdlib.h>
#include "type.h"
#include "settings.h"
#include "stimulus_file.h"

#define CONST_A 0.39874
static float getGaussianFilter(float *gaussian, float sigma, uint16 size)
{
	int i, j, index;
	float distance;
	float const_b = -1.0/(2*sigma*sigma);
	float sum = 0;

	//printf("--Gausian Filter--\n");
	index = 0;
	for (i=0;i<size*2+1;i++)
	{
		for (j=0;j<size*2+1;j++)
		{
			distance = sqrt((float)(size-i)*(size-i)+(size-j)*(size-j));
			gaussian[index] = (CONST_A * exp(distance*distance*const_b));
			//printf("%5.2f(%4.2f)", distance, gaussian[index]);
			sum += gaussian[index];
			index++;
		}
		//printf("\n");
	}

	return sum;
}

static float getGaussianValue(float sigma, uint16 distance)
{
	int i, j, index;

	float const_b = -1.0/(2*sigma*sigma);
	float gaussian;

	gaussian = (CONST_A * exp((float)distance*distance*const_b));

	return gaussian;
}

static void makeNewImage(uint16 *in, uint16 *out, uint16 size)
{
	int x, y, newX, newY;
	int newWid, newHgt;

	newWid = IRWID+size*2;
	newHgt = IRHGT+size*2;
	for (y=0; y<newHgt; y++)
	{
		for (x=0;x<newWid;x++)
		{
			newX = x-size;
			newY = y-size;

			if (newX < 0) newX = 0;
			else if (newX >= IRWID) newX = IRWID-1;

			if (newY < 0) newY = 0;
			else if (newY >= IRHGT) newY = IRHGT-1;

			out[y*newWid+x] = in[newY*IRWID+newX];
		}
	}
	//save16bit("padding.pgm", out, newWid, newHgt);
}

void bilateralFilter16(uint16 *in, uint16 *out, float sigma, uint16 size)
{
	int x, y;
	int subx, suby;
	int window_size = size*2+1;
	int filter_index;
	float filter_sum, filter_I;
	float matrix_sum;
	int newX, newY, newWid;
	uint16 gapWithCenter, centerVal, newVal;

	float *gaussian = (float*)malloc(window_size*window_size*sizeof(float));
	uint16 *newImage = (uint16*)malloc((IRWID+size*2)*(IRHGT+size*2)*sizeof(uint16));

	newWid = IRWID+size*2;
	filter_sum = getGaussianFilter(gaussian, sigma, size);
	makeNewImage(in, newImage, size);

	//printf("sigma:%4.2f size:%d, filter_sum:%5.2f\n", sigma, size, filter_sum);
	for (y=0; y<IRHGT; y++)
	{
		for (x=0;x<IRWID;x++)
		{
			matrix_sum = 0;
			filter_sum = 0;
			filter_index = 0;
			for (suby=y-size;suby<=(y+size);suby++)
			{
				for (subx=x-size;subx<=(x+size);subx++)
				{
					newX = subx + size;
					newY = suby + size;
					newVal = newImage[newY*newWid+newX];
					
					centerVal = in[y*IRWID+x];
					gapWithCenter = (centerVal>newVal)?(centerVal-newVal):(newVal-centerVal);
					filter_I = getGaussianValue(sigma*10, gapWithCenter);

					matrix_sum += (newVal*gaussian[filter_index]*filter_I);
					filter_sum += gaussian[filter_index]*filter_I;
					filter_index ++;
				}
			}
			out[y*IRWID+x] = (uint16)(matrix_sum / filter_sum);
		}
	}

	free(gaussian);
	free(newImage);
}
