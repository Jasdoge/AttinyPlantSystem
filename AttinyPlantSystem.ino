#define ATTINY

//#include <arduino.h>
#include "PlantSystem.h"
//#include "Arduino.h"

PlantSystem pSystem;

void setup(){
	
	#ifndef ATTINY
		Serial.begin(115200);
    	Serial.println("Serial run");
	#endif
	pSystem.ini();
	

}


void loop(){

	pSystem.loop();
	delay(10);

}
