#include <stdio.h>
#include <stdlib.h>
#include "type.h"
#include "settings.h"
#include "isp_core.h"
#include "stimulus_file.h"

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

			if (newX < 0) newX = -newX;
			else if (newX >= IRWID) newX = IRWID - (newX - IRWID + 1);

			if (newY < 0) newY = -newY;
			else if (newY >= IRHGT) newY = IRHGT - (newY - IRHGT + 1);

			out[y*newWid+x] = in[newY*IRWID+newX];
		}
	}
	//save16bit("padding.pgm", out, newWid, newHgt);
}

#define ABS(a) ((a)>0 ? (a) : -(a))
static void findDefect(uint16 *newImage, uint16 *kimage, uint16 size)
{
	int x, y;
	int newX, newY;
	int newWid = IRWID+size*2;
	int k, sumDiffC, sumDiffA;
	uint16 i1, i2, i3, i4, i5, i6, i7, i8, i9;

	for (y=0; y<IRHGT; y++)
	{
		for (x=0;x<IRWID;x++)
		{
			newX = x + size;
			newY = y + size;

			i1 = newImage[(newY-1)*newWid + newX-1];
			i2 = newImage[(newY-1)*newWid + newX-0];
			i3 = newImage[(newY-1)*newWid + newX+1];
			i4 = newImage[(newY-0)*newWid + newX-1];
			i5 = newImage[(newY-0)*newWid + newX-0];
			i6 = newImage[(newY-0)*newWid + newX+1];
			i7 = newImage[(newY+1)*newWid + newX-1];
			i8 = newImage[(newY+1)*newWid + newX-0];
			i9 = newImage[(newY+1)*newWid + newX+1];
			
			sumDiffC = ABS(i5-i1) + ABS(i5-i2) + ABS(i5-i3) + ABS(i5-i4)
			         + ABS(i5-i6) + ABS(i5-i7) + ABS(i5-i8) + ABS(i5-i9);
			sumDiffA = ABS(i1-i2) + ABS(i2-i3) + ABS(i3-i4) + ABS(i4-i5)
					 + ABS(i5-i6) + ABS(i6-i7) + ABS(i7-i8) + ABS(i8-i9);

			k = sumDiffC - sumDiffA;

			k = (k<K_MIN_TH) ? K_MIN_TH : (k>K_MAX_TH) ? K_MAX_TH : k;

			kimage[y*IRWID+x] = k;
		}
	}
	//save16bit("dpc_k_value.pgm", kimage, IRWID, IRHGT);
}

static void minmax(uint16 *array, uint16 size, uint16 *min, uint16 *max)
{
	int i;

	*min = *max = array[0];
	for (i=1; i<size; i++)
	{
		if (array[i] < *min) *min = array[i];
		if (array[i] > *max) *max = array[i];
	}
}

static uint16 findReplaceValue(uint16 *diffArray, uint16 *avgArray, uint16 size, uint16 min, uint16 max)
{
	int i;
	uint16 th = DIR_TH_RATIO * (max - min) / 100 + min + DIR_TH_OFFSET;
	uint16 replace = 0, count = 0;

	for (i=0;i<size; i++)
	{
		if (diffArray[i] < th) 
		{
			count++;
			replace += avgArray[i];
		}
	}

	return replace / count;
}

static void makeDPC(uint16 *newImage, uint16 *kimage, uint16 *out, uint16 size)
{
	int x, y;
	int newX, newY;
	int newWid = IRWID+size*2;
	int k, sumDiffC, sumDiffA;
	uint16 i1, i2, i3, i4, i5, i6, i7, i8, i9;
	uint16 dir[4], min_dir, max_dir, replace[5];

	for (y=0; y<IRHGT; y++)
	{
		for (x=0;x<IRWID;x++)
		{
			newX = x + size;
			newY = y + size;

			i1 = newImage[(newY-1)*newWid + newX-1];
			i2 = newImage[(newY-1)*newWid + newX-0];
			i3 = newImage[(newY-1)*newWid + newX+1];
			i4 = newImage[(newY-0)*newWid + newX-1];
			i5 = newImage[(newY-0)*newWid + newX-0];
			i6 = newImage[(newY-0)*newWid + newX+1];
			i7 = newImage[(newY+1)*newWid + newX-1];
			i8 = newImage[(newY+1)*newWid + newX-0];
			i9 = newImage[(newY+1)*newWid + newX+1];
			
			dir[0] = ABS(i2-i8);   	// NW
			dir[1] = ABS(i4-i6);   	// EW
			dir[2] = ABS(i1-i9);   	// diagonal 1
			dir[3] = ABS(i3-i7);	// diagonal 2
			minmax(dir, 4, &min_dir, &max_dir);

			replace[0] = (i2 + i8) / 2;
			replace[1] = (i4 + i6) / 2;
			replace[2] = (i1 + i9) / 2;
			replace[3] = (i3 + i7) / 2;
			replace[4] = findReplaceValue(dir, replace, 4, min_dir, max_dir);

			out[y*IRWID+x] = replace[4] * (kimage[y*IRWID+x]-K_MIN_TH)/(K_MAX_TH-K_MIN_TH)
			               + i5         * (K_MAX_TH-kimage[y*IRWID+x])/(K_MAX_TH-K_MIN_TH);
		}
	}	
}

#define FILTER_HALF 2
void autoDPC(uint16 *in, uint16 *out)
{
	uint16 *newImage = (uint16*)malloc((IRWID+FILTER_HALF*2)*(IRHGT+FILTER_HALF*2)*sizeof(uint16));
	uint16 *kimage = (uint16*)malloc(IRWID*IRHGT*sizeof(uint16));

	makeNewImage(in, newImage, FILTER_HALF);
	findDefect(newImage, kimage, FILTER_HALF);
	makeDPC(newImage, kimage, out, FILTER_HALF);

	free(newImage);
	free(kimage);
}




