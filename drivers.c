#include "settings.h"
#include "type.h"
#include "drivers.h"
#include "stimulus_file.h"

void openShutter(void)
{
    // TBD
}

void closeShutter(void)
{
    // TBD
}

void getImage(uint16 *image)
{
#ifdef _SIMULATION_
    //printf("getImage(%s)\n", filename);
    getFromFile(infilename, image);
#else
    // TBD
#endif
}

void displayOut(uint8 *rgb)
{
#ifdef _SIMULATION_
    //outToFile("output.ppm", rgb);
    outToFile(outfilename, rgb);
#else
    // TBD
#endif   
}

uint8 getCamJumper(void)
{
#ifdef _SIMULATION_
    return ULIS_QVGA;
#else
    uint8 jumper = *(uint8 volatile *)JUMPER_CAM_ADDR;
    return jumper;
#endif
}

uint8 getManufactoryJumper(void)
{
#ifdef _SIMULATION_
    char ch;
#if (MANUFACTURING > 1)
    printf("Are you manufacturing now?(y/n)");
    ch=getch();
    printf("%c\n\n", ch);
    if (ch=='y' || ch=='Y') return 1;
    else return 0;
#else
    return MANUFACTURING;
#endif
#else
    return *(volatile *)JUMPER_MANUFACTURE_ADDR;
#endif
}

void writeNucGainToFlash(void)
{
#ifdef _SIMULATION_
    saveNucGain();
#else
    // TBD
#endif
}

void readNucGainFromFlash(void)
{
#ifdef _SIMULATION_
    readNucGain();
#else
    // TBD
#endif
}

uint16 getTempInside(void)
{
#ifdef _SIMULATION_
    static uint16 sTemp = 0;

    sTemp++;
#else
    // TBD
#endif
}

uint16 getTime(void)
{
#ifdef _SIMULATION_
    static uint16 sTime = 0;

    sTime++;
#else
    // TBD
#endif
}
