#define ATTINY

#include <arduino.h>
#include "PlantSystem.h"
#include <avr/wdt.h>
#include "Arduino.h"
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
