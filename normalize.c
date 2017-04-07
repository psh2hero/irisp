#include <stdio.h>
#include <stdlib.h>
#include "type.h"
#include "settings.h"
#include "isp_core.h"

void normalize(uint16 *in, uint8 *out)
{
	int i;

    uint16 min=0xFFFF, max=0;
    uint8 normalized;

    // Get Min, Max
    for (i=0; i<IRWID*IRHGT; i++)
    {
        if (in[i] < min) min = in[i];
        if (in[i] > max) max = in[i];
    }
    

	for (i=0;i<IRWID*IRHGT;i++)
	{
		out[i] = (in[i]-min)*256.0/(float)(max-min+1);
	}
}
