#include <stdio.h>
#include <stdlib.h>
#include "type.h"
#include "settings.h"
#include "isp_core.h"

void getHistogram(in, hist)
{

}

void histEqualize(uint16 *in, uint16 *out)
{
    uint32 hist[16384];

    getHistogram(in, hist);
}
