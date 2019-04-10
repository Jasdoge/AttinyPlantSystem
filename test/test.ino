#include <arduino.h>
bool up = false;

void setup(){
	
	#ifndef ATTINY
		Serial.begin(115200);
    	Serial.println("Serial run");
	#endif

	int i;
	for(i=0; i<3; ++i )
		pinMode(i, OUTPUT);

}


void loop(){

	up = !up;

	int i;
	for(i=0; i<3; ++i )
		digitalWrite(i, up);
	delay(1500);

}
