#ifndef Cam
	#define Cam
#endif

void camInit(void);
void cam_ReadImage(short int *imgeData);
void TPM2_init(short int initMODvalue);
void TPM2_DelayOnce(void);
short int readADC(void);
short int imageData[128];
