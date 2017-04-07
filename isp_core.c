#include <stdio.h>
#include <stdlib.h>
#include "settings.h"
#include "type.h"
#include "drivers.h"
#include "isp_core.h"

float nucGain[IRWID*IRHGT];         // NUC Gain Table Array
uint16 nucOffset[IRWID*IRHGT];      // NUC Offset Table Array

void accumFrame(uint16 *in, uint32 *sumCaptureFrame)
{
    int i;
    
    for (i=0;i<IRWID*IRHGT;i++)
    {
        sumCaptureFrame[i] = sumCaptureFrame[i] + in[i];
    }    
}

void makeNucGain(uint32 *hotImage, uint32 *coldImage)
{
    int i;
    
    uint32 hotSum = 0, coldSum = 0;
    uint32 hotAvg, coldAvg;

    // Average of Hot, Cold Image
    for (i=0;i<IRWID*IRHGT;i++)
    {
        hotSum += hotImage[i];
        coldSum += coldImage[i];
    }
    hotAvg = hotSum / (IRWID*IRHGT);
    coldAvg = coldSum / (IRWID*IRHGT);

    // Calculate NUC Gain Table
    for (i=0;i<IRWID*IRHGT;i++)
    {
        nucGain[i] = (float)(hotAvg - coldAvg) / (float)(hotImage[i] - coldImage[i]);
    }
}

void makeNucOffset(uint16 *in)
{
    int i;
    
    uint32 sum = 0;
    uint16 avg;
    
    for (i=0;i<IRWID*IRHGT;i++) 
    {
        sum = sum + in[i];
    }
    avg = (uint16)(sum / (IRWID*IRHGT));
    
    for (i=0;i<IRWID*IRHGT;i++)
    {
        nucOffset[i] = (uint16)((float)avg - nucGain[i] * (float)in[i]);
    }    
}

void doNUC(uint16 *in, uint16 *out)
{
    int i;
    
    printf("----doNUC----.\n");
    for (i=0;i<IRWID*IRHGT;i++)
    {
        out[i] = (uint16)((float)in[i] * nucGain[i] + (float)nucOffset[i]);
    }
}

void doDPC(uint16 *in, uint16 *out)
{
    int i, j;

	uint16 *tmp = (uint16*)malloc(IRWID*IRHGT*sizeof(uint16));

    printf("----doDPC----.\n");
	//simpleDPC(in, tmp);
	autoDPC(in, out);

	free(tmp);
}

void do2DNR(uint16 *in, uint16 *out)
{
	printf("----do2DNR----.\n");
    //gaussianFilter16(in, out, 0.8, 2);
    bilateralFilter16(in, out, 1, 2);
}

void do3DNR(uint16 *in, uint16 *out)
{
    // TBD
    printf("----do3DNR----.\n");
}

#include "palette_gray.h"
void paletteMapping(uint16 *in, uint8 *rgb)
{
    int i;

    // Convert 16bit gray to rgb
    for (i=0; i<IRWID*IRHGT; i++)
    {
        rgb[i*3+0] = palette[in[i]*3+0];
        rgb[i*3+1] = palette[in[i]*3+1];
        rgb[i*3+2] = palette[in[i]*3+2];
    }

//    printf("paletteMapping.\n");
}
