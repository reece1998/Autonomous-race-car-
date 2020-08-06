// center aligned pwn	
#include <MKL25Z4.H>
#include "Servo.h"
#include "Motor.h"

void motor (short speed) {
	SIM->SCGC5 |= 0x2800; //enable clock to port c	and port e
	
	SIM->SCGC6 |= 0x01000000; // enable clock to TMP0
	SIM->SOPT2 |= 0x01000000; // use MCGFLLCLK as timer counter clock
	
	PORTC->PCR[1] = 0x0400; // PTD1 used by TPM0	forwards
	PORTC->PCR[3] = 0x0400; // PTC3 used by TPM0  forwards
	//PORTC->PCR[2] = 0x0400;  PTC2 used by TPM0  backwards
	//PORTC->PCR[4] = 0x0400;  PTC4 used by TPMO  backwardS
	
	PORTE->PCR[21] = 0x0100; // Enable H-BRIDGE_ENABLE
	
	PTE->PDDR |= 0x200000; //CONFI pte as output
	PTE->PDOR |= 0x200000; // set output as 1
	PTE->PSOR |= 0x200000; //setting the output as 1
	PTC->PDDR |= 0x1E;

	TPM0->SC = 0; //disable timer
	TPM0->CONTROLS[0].CnSC = 0x20 | 0x08; // Edge-aligned, pulse high FORWARDS
	TPM0->CONTROLS[2].CnSC = 0x20 | 0x08; // Edge-aligned, pulse high FORWARDS
	//TPM0->CONTROLS[1].CnSC = 0x20 | 0x08; // Edge-aligned, pulse high BACKWARDS
	//TPM0->CONTROLS[3].CnSC = 0x20 | 0x08; // Edge-aligned, pulse high BACKWARDS

	TPM0->MOD = 21969; // Set up modulo register 131 max    //1000 100

	TPM0->CONTROLS[0].CnV = speed; // Set up channel value for 60% duty cycle FORWARDS
	TPM0->CONTROLS[2].CnV = speed; // Set up channel value for 60% duty cycle FORWARDS
	//TPM0->CONTROLS[1].CnV = 78; // Set up channel value for 60% duty cycle BACKWARDS
	//TPM0->CONTROLS[3].CnV = 78; // Set up channel value for 60% duty cycle BACKWARDS
	
	TPM0->SC = 0x0B ; // enable TPM0 with prescaler /8, Edge-aligned // you removed the 0x02
	TPM0->SC |= 0x80; // Clear TOF
	TPM0->SC |= 0x08; // Enable timer free running mode


		//while(1) 
		//{
			//while((TPM0->SC & 0x80) == 0)
			//{}
		//TPM0->SC |= 0x80; //Clear TOF
	//}
}	
