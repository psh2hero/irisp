#include <stdio.h>
#include <stdlib.h>
#include "settings.h"
#include "type.h"
#include "drivers.h"
#include "stimulus_file.h"
#include "isp_core.h"

#ifdef _SIMULATION_
char infilename[50]={0,};
char outfilename[50]={0,};
#endif

void doShutter(void)
{
    uint16 captureImage[IRWID*IRHGT];       // Capture Image Array 

    closeShutter();

#ifdef _SIMULATION_
    strcpy(infilename, "cold.txt");
#endif   
    getImage(captureImage);
    makeNucOffset(captureImage);
    
    openShutter();
}

void initNucOffset(void)
{
    doShutter();                // do shutter for updating NUC Offset table.
}

void doCalcNucGain(void)
{
    char ans = 1;
    int i;
    uint32 hotImage[IRWID*IRHGT] = {0, };
    uint32 coldImage[IRWID*IRHGT] = {0, };
    uint16 captureImage[IRWID*IRHGT];       // Capture Image Array 

    printf("To Make NUC Gain, follow the instructions, please.\n");

    /* Capture Hot Image */
    printf("Set the blackbody to hot temperature.(Normally 50 degree)\n");

    while (ans) {
        printf("Does the blackbody set to the hot temperature?\n");
        printf("if yes, press '0'.\n");
        ans = getch()-'0';
    }
#ifdef _SIMULATION_
    strcpy(infilename, "hot.txt");
#endif    
    // Cature Hot Image of 100 frames
    for (i=0;i<NUC_FRAME_NUM;i++)
    {
        getImage(captureImage);             // Capture One Frame from Detector.
        accumFrame(captureImage, hotImage);
    }

    /* Capture Cold Image */
    printf("Set the blackbody to cold temperature.(Normally 20 degree)\n");

    ans = 1;
    while (ans) {
        printf("Does the blackbody set to the cold temperature?\n");
        printf("if yes, press '0'.\n");
        ans = getch()-'0';
    }
#ifdef _SIMULATION_
    strcpy(infilename, "cold.txt");
#endif       
    // Cature Cold Image of 100 frames
    for (i=0;i<NUC_FRAME_NUM;i++)
    {
        getImage(captureImage);             
        accumFrame(captureImage, coldImage);
    }

    // Make NUC Gain Table
    makeNucGain(hotImage, coldImage);
}

void showImage(uint16 *image)
{
    uint8 rgb[IRWID*IRHGT*3];
    uint8 contrast[IRWID*IRHGT];

    doContrast(image, contrast);
    paletteMapping(contrast, rgb);

    displayOut(rgb);
    printf("showImage.\n");
}

void initSetting(int cam)
{
#ifndef _SIMULATION_
    switch (cam)
    {
        case ULIS_QVGA:
            /* ULIS QVGA Detector Initial Settings */
            break;
        case DRS_QVGA:
            /* DRS Settings */
            break;
        case I3_IR:
            /* I3 Settings */
            break;
        case UE_IR:
            /* UE Settings */
            break;
        default:
            break;
    }
#endif
}

bool connectCamera(int *ir)
{
    *ir = getCamJumper();
    
    if (*ir == NO_DETECTOR) return false;

    return true;
}

bool checkManufacture(void)
{
    return getManufactoryJumper();
}

bool checkShutterNeed(void)
{
    static int temp = 0;
    static int time = 0;

    int newTemp, newTime;
    newTemp = getTempInside();      // Get the current temperature inside the IR Detector.
    newTime = getTime();            // Get the current time

    if ((abs(temp - newTemp) > SHUTTER_TEMP) || ((newTime - time) > SHUTTER_TIME))
    {
        temp = newTemp;
        time = newTime;
        return true;
    }

    return false;
}

int main(void)
{
    uint16 captureImage[IRWID*IRHGT];       // Capture Image Array 
    uint16 displayImage[IRWID*IRHGT];       // Display Image Array
    uint16 nucImage[IRWID*IRHGT];           // NUC processed Image Array
    uint16 dpcImage[IRWID*IRHGT];           // DPC processed Image Array
    uint16 nr2dImage[IRWID*IRHGT];          // 2DNR processed Image Array
    uint16 nr3dImage[IRWID*IRHGT];          // 3DNR processed Image Array

    int ir;             // Camera Id pointer.
    bool shutterNeed;
#if 1
    // Connect IR Camera
    if (!connectCamera(&ir)) 
    {
        printf("There is no available camera. Please check out.\n");
        return 0;
    }

    // Activate the Camera
    initSetting(ir);            // Set initial registers.

    if (checkManufacture())        // flag is true when it is manufacturing.
    {
        doCalcNucGain();        // Calculate NUC Gain Table.
        writeNucGainToFlash();  // Write NUC Gain Table to Flash Memory.
    }
    else
    {
        readNucGainFromFlash(); // Read NUC Gain Table from Flash Memory.
    }
    initNucOffset();            // Calculate NUC Offset Table.

    // Working Sequence
#ifndef  _SIMULATION_    
    while (1)
#endif
    {
        shutterNeed = checkShutterNeed();   // check if shuttering is needed.
        if (shutterNeed)
        {
            doShutter();                    // do shutter for updating NUC Offset table.
            /* Freeze Effect without updating displayImage */
        }
        else
        {
#ifdef _SIMULATION_
            strcpy(infilename, "face_raw.txt");
#endif                
            getImage(captureImage);             // Capture One Frame from Detector.
            doNUC(captureImage, nucImage);  // NUC Processing
            doDPC(nucImage, dpcImage);      // DPC Processing
            do2DNR(dpcImage, nr2dImage);    // 2DNR Processing
            //do3DNR(nr2dImage, nr3dImage);   // 3DNR Processing
        }

#ifdef SAVE_RAW_IMAGE
        strcpy(outfilename, "out_raw.ppm");
        showImage(captureImage);
#endif   
#ifdef SAVE_NUC_IMAGE
        strcpy(outfilename, "out_nuc.ppm");
        showImage(nucImage);
#endif   
#ifdef SAVE_DPC_IMAGE
        strcpy(outfilename, "out_dpc.ppm");
        showImage(dpcImage);
#endif   
#ifdef SAVE_2DNR_IMAGE
        strcpy(outfilename, "out_2dnr.ppm");
        showImage(nr2dImage);
#endif   
#ifdef _SIMULATION_        
        strcpy(outfilename, "output.ppm");
#endif
        showImage(nr2dImage);            // Display Image
    }
#endif
    return 1;
}

