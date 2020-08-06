#include <MKL25Z4.H>
#include "Servo.h"
#include "Motor.h"
#include "cam.h"
////////////////////////////////////////////////////////////////////////////////////////
void movement(short int *imgeData);
short int imageData[128];
int main(void){
while (1) 
		{
			camInit();
			cam_ReadImage(imageData);
			movement(imageData);
			}
}

////////////////////////////////////////////////////////////////////////////////////////		
void movement(short int *imgeData){ 
	short int o,i,angle,ctrl=0,curSpeed,speed=1000, cL=0, cR=0, curAngle=0;
	short int cL1=0,cL2=0,cL3=0,cR1=0,cR2=0,cR3=0;

	motor(speed);

////////////////////////////////////////////////////////////////////////////////////////
	for(i=16;i<64;i++){//counts the amount of pixles in the left under cL
	o=imgeData[i];
	if(o<4000){
	cL++;
	}
	}
	for(i=64;i<113;i++){//counts the amount of pixles in the right cR
	o=imgeData[i];
	if(o<4000){
	cR++;
	}
	}
////////////////////////////////////////////////////////////////////////////////////////
	if(cL>cR){	
		for(i=16;i<32;i++){//counts the amount of pixles in the most far left of cL 0-21
		o=imgeData[i];
		if(o<4000){
		cL1++;
		}
		}
		for(i=34;i<49;i++){//counts the amount of pixles in the left of cL
		o=imgeData[i];
		if(o<4000){
		cL2++;
		}
		}
		for(i=50;i<64;i++){//counts the amount of pixles in the middle of cL
		o=imgeData[i];
		if(o<3900){
		cL3++;
		}
		}
		if(cL1>cL2&cL1>cL3){//stright
		angle=1966;
		ctrl=1;
		}
		if(cL2>cL1&cL2>cL3){//LR
		angle=2158;
		ctrl=1;
		}
		if(cL3>cL1&cL3>cL2){//R
		angle=2350;
		ctrl=1;
		}
	}

////////////////////////////////////////////////////////////////////////////////////////
	if(cR>cL){	
		for(i=65;i<78;i++){//counts the amount of pixles in the middle cR
		o=imgeData[i];
		if(o<4000){
		cR3++;
		}
		}
		for(i=79;i<95;i++){//counts the amount of pixles in the left of cR
		o=imgeData[i];
		if(o<4000){
		cR2++;
		}
		}
		for(i=96;i<112;i++){//counts the amount of pixles in far right of cR
		o=imgeData[i];
		if(o<4000){
		cR1++;
		}
		}	
		if(cR1>cR2&cR1>cR3){//stright
		angle=1966;
		ctrl=1;
		}
		if(cR2>cR1&cR2>cR3){//LR
		angle=1774;
		ctrl=1;
		}
		if(cR3>cR1&cR3>cR2){//R
		angle=1582;
		ctrl=1;
		}
		}
	if (cL==cR){
	angle=1966;
	}
////////////////////////////////////////////////////////////////////////////////////////
//Left=1582	LiLeft=1774 	Forward=1966 	LiRight=2158 		Right=2350
//angle=/=curAng 
//check for current values
////////////////////////////////////////////////////////////////////////////////////////
		if(curAngle==angle){
			ctrl=0;
		}
		else {curAngle = angle;}
		switch(ctrl)
{
	case 1:
	servo(angle);
	
			break;
	case 0:
	{}
		break;
	default:
	{}
		break;
}
//vaiabler that holds current value
}
////////////////////////////////////////////////////////////////////////////////////////	