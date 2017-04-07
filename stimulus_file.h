#include "settings.h"
#include "type.h"

void getFromFile(char* filename, uint16 *img);
void outToFile(char *filename, uint8 *rgb);
void saveNucGain(void);
void readNucGain(void);
void save16bit(char *filename, uint16 *raw16, uint16 wid, uint16 hgt);

extern char infilename[50];
extern char outfilename[50];
