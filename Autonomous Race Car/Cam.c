#include "MKL25Z4.h"
#include "stdio.h"
#include "Servo.h"
#include "Motor.h"

#define CAM_SI_HIGH PTD->PSOR=(0x01<<7); //The SI start singal for the camera 'HIGH'
#define CAM_SI_LOW PTD->PCOR=(0x01<<7);  //The SI start signal for the camera 'LOW'

#define CAM_CK_HIGH PTE->PSOR=(0x01<<1); //Set the clock input to the camera as 'HIGH'
#define CAM_CK_LOW PTE->PCOR=(0x01<<1);  //Set the clock input to the camera as 'LOW'

#define FRQ_MCGFLLCLK 20971520 

short int readADC(void);

void camInit(void);
void cam_ReadImage(short int *imgeData);
void TPM0_init(short int initMODvalue);
void TPM0_DelayOnce(void);


/*int main (void)
{
  while(1){  
    
	 // char buf [100];   // UART buffer 100
    int n,i;            // number of characters in b2097uf to be sent
	
   // int nImgRd; // number of image reading (how many times has the image been red)    
		

	
    
	  1)camInit();  // configure camera
	  //nImgRd=0;
	
    
	// initialize TPM0 for 1 us delay
	  
		
 	
	  
	
			n=sprintf(buf, "nImgRd=%d\r\n", nImgRd);
			sendStr(buf, n);
			nImgRd++;			
		  2)cam_ReadImage(imageData); //in c an array could only be moved into a function using a pointer 
			3)testing(imageData);
		  
			// send the image data to PC
			for (i=0;i<128;i++)
			{	n = sprintf(buf, "%d ", imageData[i]); // convert integer value into ASCII
			  sendStr(buf, n);
			}
			sendStr("\r\n \r\n", 5); 
			
		  

    }
	} */

		
#define DIFF_SINGLE 0x00
#define DIFF_DIFFERENTIAL (0x01<<5)


void camInit(void)
{ 
	short unsigned int initMODValue;
		initMODValue=0.5*(float)(FRQ_MCGFLLCLK)/1000000.0; //MOD value for 1us delay
	  TPM0_init(initMODValue);
	//Initial the IO port
	
	// SI (PTD7) output, CLK (PTE1) output
	// AO (PTD5) input
	// SIM->SCGC5 |= (0x1<<10); // enable clock to PTB
	
	SIM->SCGC5 |=(0x1<<12 | 0x1<<13); 
  /* enable clock to Port D, E */
	
    PORTD->PCR[7] = 0x100;     /* make PTD7 pin as GPIO */
    PTD->PDDR |= (0x1<<7);     /* make PTD7 as output pin */
	
    PORTE->PCR[1] = 0x100;     /* make PTE1 pin as GPIO */
    PTE->PDDR |= (0x1<<1);     /* make PTE1 as output pin */
	
		PORTD->PCR[5] = 0; // PTD5.MUX[10 9 8]=000, analog input
		PTD->PDDR |= 0x00;
	
	  SIM->SCGC6 |= 0x08000000;   // enable clock to ADC0 ; 0x8000000u
	
	
	  // Configure ADC as it will be used, but because ADC_SC1_ADCH is 31,
    // the ADC will be inactive.  Channel 31 is just disable function.
    // There really is no channel 31.
	  // disable AIEN, Signle-ended, channel 31
    ADC0->SC1[0] = DIFF_SINGLE|ADC_SC1_ADCH(31);  
		// TODO: use hardware triger, TPM0
	  ADC0->SC2 &= ~0x40;   // ADTRG=0, software trigger
	
	  // clock div by 4, long sample time, single ended 12 bit, bus clock 
    ADC0->CFG1 =(0x1<<6 | 0x1<<4 |0x1<<2); //0b01010100; 
	  //  ADC0->CFG1 = 0x40 | 0x10 | 0x04 | 0x00;
	
	  // select the B set of ADC input channels for PTD5 (SE6b)
	  ADC0->CFG2 |=(0x1<<4); //CFG2.MUXSEL=1, ADxxb channels are selected; 
	
	  CAM_SI_LOW;
	  CAM_CK_LOW;
	
		
}   
		
void cam_ReadImage(short int *imgeData)//taking the array and copying the address into a pointer (reason is that you could not save an array into another parameter
{  
	unsigned int i;
	// SI (PTD7) Digital output, CLK (PTE1) Digitaloutput
	// AO (PTD5) Analgoue input (channel 6)
	
	 CAM_SI_HIGH;
	 TPM0_DelayOnce();
	 CAM_CK_HIGH;
	 TPM0_DelayOnce();
	 CAM_SI_LOW;
	 TPM0_DelayOnce();
	 
	// the following reading sequence is different from the diagram
	// Fig. 11, page 10, AMU's TLS1401CL.pdf. 
	// image sensor is red at the falling edge of CLK
	// howver, Fig. 11 suggests reading image at rising edge
	// read the 128 pixel image data
	 // imgData[0]=(short int)readADC(6); 
	 // CAM_CK_LOW;
			CAM_CK_LOW;
		TPM0_DelayOnce();
		TPM0_DelayOnce();
		
	for (i=0;i<128;i++)
	{
		imgeData[i]=(short) readADC(); //saving different values to the addresses within the array in the pointer *imgData to save data in relation with the address on the array insted of saving the variables in the array
		CAM_CK_HIGH;
		TPM0_DelayOnce();
		TPM0_DelayOnce();
		CAM_CK_LOW;
		TPM0_DelayOnce();
		TPM0_DelayOnce();
	}
	
	// additional one CLK to allow for integration and to leave place for the next one
	  CAM_CK_HIGH;
		TPM0_DelayOnce();
		TPM0_DelayOnce();
		CAM_CK_LOW;
	  TPM0_DelayOnce();
		TPM0_DelayOnce();
}   
		

		
short int readADC(void) 
{
	short int result;     	
	
	ADC0->SC1[0] = 6; //software triger conversion on channel 6, SE13
	while(!(ADC0->SC1[0] & 0x80)) { } /* wait for conversion complete */
	result = ADC0->R[0];        /* read conversion result and clear COCO flag */
	return result;
}
// Now a function to transfer the camera data into the servo and motor

//Left=1582	LiLeft=1774 	Forward=1966 	LiRight=2158 		Right=2350	
// Initialize the TPM0 to generate a specified delay in number of MCGFLLCLK clocks
// By default, the MCGFLLCLK set by system setup is 20.97152MHz
void TPM0_init(short int initMODvalue)
{
	
	SIM->SCGC6 |= (0x01<<24); // 0x01000000;, enale clk to TPM0
	SIM->SOPT2 |=(0x01<<24); // 0x01000000, use MCGFLLCLK as timer counter clk
	TPM0->SC = 0; // diable timer when configuring
	// TPM0->MOD = 0xFFFF; // by default, set the 16-bit modulo value to maximum
	                    // thus results in maximum delay
	TPM0->MOD = initMODvalue;
	TPM0->SC|=0x80; // clear TOF
	// TPM0->SC|=0x08; // enable timer free-rnning mode
}

// Initialize the TPM0 to generate a specified delay in number of MCGFLLCLK clocks
// By default, the MCGFLLCLK set by system setup is 20.97152MHz

void TPM0_DelayOnce(void)
{
	TPM0->SC|=0x80; // clear TOF
	TPM0->SC|=0x08; // enable timer free-rnning mode
	while((TPM0->SC & 0x80) == 0) { } // wait until the TOF is set
	TPM0->SC = 0; // diable timer when configuring
}


