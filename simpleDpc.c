#include <stdio.h>
#include <stdlib.h>
#include "type.h"
#include "settings.h"
#include "isp_core.h"
#include "stimulus_file.h"

static void makeDPC(int y, int x, uint16 *in, uint16 *out)
{
    if (x==0 && y==0)                       // TOP, LEFT Point
    {
        out[0] = (in[1] + in[IRWID] + in[IRWID+1])/3;
    }
    else if (x==(IRWID-1) && y==0)          // TOP, RIGHT Point
    {
        out[IRWID-1] = (in[IRWID-2] + in[IRWID*2-2] + in[IRWID*2-1])/3;
    }
    else if (y==0)                          // TOP Line
    {
        out[x] = (in[x-1] + in[x+1] + in[IRWID+x-1] + in[IRWID+x] + in[IRWID+x+1])/5;
    }
    else if (x==0 && y==(IRHGT-1))          // BOTTOM, LEFT Point
    {   
        out[(IRHGT-1)*IRWID] = (in[(IRHGT-2)*IRWID] + in[(IRHGT-2)*IRWID+1] + in[(IRHGT-1)*IRWID+1])/3;
    }
    else if (x==(IRWID-1) && y==(IRHGT-1))  // BOTTOM, RIGHT Point
    {
        out[IRHGT*IRWID-1] = (in[IRHGT*IRWID-2] + in[(IRHGT-1)*IRWID-1] + in[(IRHGT-1)*IRWID-2])/3;
    }
    else if (y==(IRHGT-1))                  // BOTTOM Line
    {
        out[(IRHGT-1)*IRWID + x] = (in[(IRHGT-1)*IRWID+x-1] + in[(IRHGT-1)*IRWID+x+1] + in[(IRHGT-2)*IRWID+x-1] + in[(IRHGT-2)*IRWID+x] + in[(IRHGT-2)*IRWID+x+1])/5;
    }
    else if (x==0)                          // LEFT Line
    {
        out[y*IRWID] = (in[(y-1)*IRWID] + in[(y+1)*IRWID] + in[(y-1)*IRWID + 1] + in[y*IRWID+1] + in[(y+1)*IRWID+1])/5;
    }
    else if (x==(IRWID-1))                  // RIGHT Line
    {
        out[y*IRWID+IRWID-1] = (in[(y-1)*IRWID+IRWID-1] + in[(y+1)*IRWID+IRWID-1] + in[(y-1)*IRWID+IRWID-2] + in[y*IRWID+IRWID-2] + in[(y+1)*IRWID+IRWID-2])/5;
    }
    else                                    // Middle AREA
    {
        out[y*IRWID+x] = (in[(y-1)*IRWID+(x-1)]+in[(y-1)*IRWID+x]+in[(y+1)*IRWID+(x+1)]+in[y*IRWID+x-1]+in[y*IRWID+x+1]+in[(y-1)*IRWID+x-1]+in[(y-1)*IRWID+x]+in[(y-1)*IRWID+x+1])/8;
    }
}

extern float nucGain[IRWID*IRHGT];
void simpleDPC(uint16 *in, uint16 *out)
{
    int i, j;

    for (i=0; i<IRHGT; i++)
    {
        for (j=0; j<IRWID; j++)
        {
            if ((nucGain[i*IRWID+j] > DPC_HIGH_TH) || (nucGain[i*IRWID+j] < DPC_LOW_TH))
            {
                makeDPC(i, j, in, out);  
                //printf("DPC - %3d, %3d (%f)\n", j, i, nucGain[i*IRWID+j]);
            } 
            else out[i*IRWID + j] = in[i*IRWID + j];
        }
    }
}

