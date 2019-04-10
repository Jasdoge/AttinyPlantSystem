/*
	Todo:
		- Design case
			- 3pin connector for sensor
			- 2 pin connector for battery
			- 2 pin connector for motor
			- Maybe fit it on the can itself?
			- Button to toggle calibration mode
			- Debug LED
		
*/

#define ATTINY

#include <arduino.h>
#include "PlantSystem.h"
#include <avr/wdt.h>
ISR(WDT_vect) {
	wdt_disable();  // disable watchdog
}



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