#include "type.h"

#define JUMPER_CAM_ADDR         0x40000000 // example psuedo address
#define JUMPER_MANUFACTURE_ADDR 0x41000000 // example psuedo address

void openShutter(void);
void closeShutter(void);

void getImage(uint16 *image);
void displayOut(uint8 *rgb);

uint8 getCamJumper(void);
uint8 getManufactoryJumper(void);

void writeNucGainToFlash(void);
void readNucGainFromFlash(void);

uint16 getTempInside(void);
uint16 getTime(void);

