#include "type.h"

void accumFrame(uint16 *in, uint32 *sumCaptureFrame);

void makeNucGain(uint32 *hotImage, uint32 *coldImage);
void makeNucOffset(uint16 *in);

void doNUC(uint16 *in, uint16 *out);
void doDPC(uint16 *in, uint16 *out);
void do2DNR(uint16 *in, uint16 *out);
void do3DNR(uint16 *in, uint16 *out);

void paletteMapping(uint16 *in, uint8 *rgb);

//2DNR//////////////////////////////////////////////////////////////////////
void gaussianFilter16(uint16 *in, uint16 *out, float sigma, uint16 size);
void bilateralFilter16(uint16 *in, uint16 *out, float sigma, uint16 size);

//DPC///////////////////////////////////////////////////////////////////////
void simpleDPC(uint16 *in, uint16 *out); 
void autoDPC(uint16 *in, uint16 *out);

//Contrast//////////////////////////////////////////////////////////////////
void normalize(uint16 *image, uint16 *contrast);
void histEqualize(uint16 *image, uint16 *contrast);


