#include <stdio.h>
#include <stdlib.h>
#include "type.h"
#include "settings.h"
#include "isp_core.h"

void getHistogram(uint16 *in, uint32 *hist)
{
    int i;

    memset(hist, 0, 16384*sizeof(uint32));

    for (i=0;i<IRWID*IRHGT;i++)
    {
        hist[in[i]]++;
    }

}

#define HIST_TH 2
void accumHistogram(uint32 *hist, uint32 *accumHist)
{
	int i;

	accumHist[0] = hist[0];
    for (i=1;i<16384;i++)
    {
        accumHist[i] = accumHist[i-1] +((hist[i]>HIST_TH)?HIST_TH:hist[i]);
    }

}

void histEqualize(uint16 *in, uint8 *out)
{
	int i;

    uint32 hist[16384];
    uint32 accumHist[16384];

    getHistogram(in, hist);
    accumHistogram(hist, accumHist);

    for (i=0;i<IRWID*IRHGT;i++)
    {
    	out[i] = accumHist[in[i]]*256.0/(accumHist[16383]+1);
    }
}
