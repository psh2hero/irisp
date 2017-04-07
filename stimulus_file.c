#include <stdio.h>
#include "settings.h"
#include "type.h"
#include "stimulus_file.h"

void getFromFile(char* filename, uint16 *img)
{
    FILE *fin = NULL;
    int i;

    fin = fopen(filename, "rt");
    if (fin == NULL)
    {
        printf("getFromFile Error : %s cannot open.\n", filename);
        memset(img, 0, IRWID*IRHGT*2);
        return;
    }
    for (i=0;i<IRWID*IRHGT;i++)
    {
        fscanf(fin, "%d,", img+i);
    }
    fclose(fin);

    printf("File Open : %s\n", filename);
}

void outToFile(char *filename, uint8 *rgb)
{
    FILE *fout = NULL;
    int i;

    fout = fopen(filename, "wt");
    if (fout == NULL)
    {
        printf("outToFile Error : %s cannot open.\n", filename);
        return;
    }

    fprintf(fout, "P3\n%d %d\n255\n", IRWID, IRHGT);
    for (i=0;i<IRWID*IRHGT;i++)
    {
        fprintf(fout, "%d %d %d\n", rgb[i*3+0], rgb[i*3+1], rgb[i*3+2]);
    }
    fclose(fout);
    printf("File Save : %s\n", filename);
}

extern float nucGain[IRWID*IRHGT];
void saveNucGain(void)
{
    FILE *fout = NULL;
    int i;

    fout = fopen("nucGain.txt", "wt");
    if (fout == NULL)
    {
        printf("saveNucGain Error : nucGain.txt cannot open.\n");
        return;
    }

    for (i=0;i<IRWID*IRHGT;i++)
    {
        fprintf(fout, "%5.4f\n", nucGain[i]);
    }
    fclose(fout);
    printf("Nuc Gain File(nucGain.txt) Saved!\n");
}

void readNucGain(void)
{
    FILE *fin = NULL;
    int i;

    fin = fopen("nucGain.txt", "rt");
    if (fin == NULL)
    {
        printf("readNucGain Error : nucGain.txt cannot open.\n");
        return;
    }

    for (i=0;i<IRWID*IRHGT;i++)
    {
        fscanf(fin, "%f\n", nucGain+i);
    }
    fclose(fin);
    printf("Nuc Gain File Read!\n");
}

void save16bit(char *filename, uint16 *raw16, uint16 wid, uint16 hgt)
{
    FILE *fout = NULL;
    int i;

    fout = fopen(filename, "wt");
    if (fout == NULL)
    {
        printf("save16bit Error : %s cannot open.\r", filename);
        return;
    }

    fprintf(fout, "P2\n%d %d\n16383\n", wid, hgt);
    for (i=0;i<wid*hgt;i++)
    {
        fprintf(fout, "%05d\n", raw16[i]);
    }
    fclose(fout);
    printf("%s Saved!\n", filename);
}
