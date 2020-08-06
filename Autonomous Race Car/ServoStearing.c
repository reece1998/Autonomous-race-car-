// Responsible for steering control
#include <MKL25Z4.H>
#include "Servo.h"
#include "Motor.h"


void servo (short angle) 
	{
	///////////////////////Start of Servo Config///////////////////////
	SIM->SCGC5 |= 0x0400; //Enable clock to Port B
	SIM->SCGC6 |= 0x02000000; //Enable clock to TPM1
	SIM->SOPT2 |= 0x01000000; //Use MCGFLLCLK as timer counter clock
		
	PORTB->PCR[0] = 0x300; //Set PTB1 pin for TPM1CH0
	PTB->PDDR=0x01; //Config PTB0 as output
	
	//How to get MOD:
	//F=Clock/Prescaler*Mod*2
	//Rearrange for MOD therefore
	//MOD=Clock/Prescaler*F*2
	//Left=1ms	LiLeft=1.25ms	Forward=1.5ms	LiRight=1.75ms	Right=2ms
	//Left=1582	LiLeft=1774 	Forward=1966 	LiRight=2158 		Right=2350
		
	TPM1->SC = 0;	//Disable timer while configuring 
	TPM1->SC = 0x0B | 0x20;	// Enable TPM1 with prescaler /8, center-aligned
	TPM1->CONTROLS[0].CnSC = 0x20 | 0x08;	// Center-aligned, pulse high 
	TPM1->MOD= 26213; // Modulo value period between the rising edge
	TPM1->SC |= 0x80;	//Clear TOF 
	TPM1->SC |= 0x08; //Enable timer free-running mode
	///////////////////////End of Servo Config///////////////////////	
		
	TPM1->CONTROLS[0].CnV = angle ;	// Sets up the pulse width

		//while (1) 
		{
      while((TPM1->SC & 0x80) == 0) 
		{ }	
		TPM1->SC |= 0x80; 		//Clear TOF 
		}
		
}
