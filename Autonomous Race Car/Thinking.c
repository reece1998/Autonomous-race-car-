for(i=0;i<64;i++){//counts the amount of pixles in the left under cL
	o=imgeData[i];
	if(o<1000){
	cL++;
	}
for(i=64;i<128;i++){//counts the amount of pixles in the right cR
	o=imgeData[i];
	if(o<1000){
	cR++;
	}
////////////////////////////////////////////////////////////////////////////////////////
if(cL>cR){	
for(i=0;i<21;i++){//counts the amount of pixles in the most far left of cL 0-21
	o=imgeData[i];
	if(o<1000){
	cL1++;
	}
for(i=21;i<43;i++){//counts the amount of pixles in the left of cL
	o=imgeData[i];
	if(o<1000){
	cL2++;
	}
for(i=43;i<64;i++){//counts the amount of pixles in the middle of cL
	o=imgeData[i];
	if(o<1000){
	cL3++;
	}	
if(cL1>cL2|cL3){//stright
	angle=1966;
	ctrl=1;
}
	if(cL2>cL3|cL1){//LR
		angle=2158;
	ctrl=1;
}
if(cL3>cL1|cL2){//R
		angle=2350;
	ctrl=1;
}
}
}
}
}
////////////////////////////////////////////////////////////////////////////////////////
if(cR>cL){	
for(i=64;i<85;i++){//counts the amount of pixles in the middle cR
	o=imgeData[i];
	if(o<1000){
	cR3++;
	}
for(i=85;i<107;i++){//counts the amount of pixles in the left of cR
	o=imgeData[i];
	if(o<1000){
	cR2++;
	}
for(i=107;i<128;i++){//counts the amount of pixles in far right of cR
	o=imgeData[i];
	if(o<1000){
	cR1++;
	}	
if(cR1>cR2|cR3){//stright
	angle=1966;
	ctrl=1;
}
	if(cR2>cR3|cR1){//LR
		angle=1774;
		ctrl=1;
}
if(cR3>cR1|cR2){//R
		angle=1582;
		ctrl=1;
}
}
}
}
}
//////////////////////	
//Left=1582	LiLeft=1774 	Forward=1966 	LiRight=2158 		Right=2350	
//////////////////////	