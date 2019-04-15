#define ATTINY

#include "PlantSystem.h"
#include <avr/sleep.h>
#include <avr/wdt.h>
#include <Arduino.h>

#define adc_disable() ADCSRA &= ~ bit(ADEN) // disable ADC (before power-off)
#define adc_enable()  (ADCSRA |=  (1<<ADEN)) // re-enable ADC

PlantSystem::PlantSystem(){}

void PlantSystem::ini(){

	adc_disable();	// ADC pulls a lot of current, without it, we get down to ~1uA while sleeping
	pinMode(pin_pump, OUTPUT);
	pinMode(pin_sensor_output, OUTPUT);
	pinMode(pin_sensor_input, INPUT);
	pinMode(pin_config_button, INPUT);
	pinMode(pin_debug_led, OUTPUT);
	digitalWrite(pin_pump, LOW);
	digitalWrite(pin_sensor_output, LOW);
	digitalWrite(pin_debug_led, HIGH);		// Connected on cathode
	is_debug_mode = digitalRead(pin_config_button) == LOW;

	if( is_debug_mode ){

		digitalWrite(pin_sensor_output, HIGH);
		bool h = true;	// LED lights while LOW, so start at HIGH
		for( uint8_t i=0; i<5; ++i ){
			h = !h;
			digitalWrite(pin_debug_led, h);
			delay(250);
		}
	}
}

void PlantSystem::togglePump( bool on ){
	
	digitalWrite(pin_pump, on);
	digitalWrite(pin_debug_led, !on);
	if( on )
		pump_started = millis();
	else
		pump_started = 0;
	
}

// Starts watering the plants
void PlantSystem::waterPlants(){
	togglePump(true);
}


void PlantSystem::enableWatchdog(const byte interval){  

	MCUSR = 0;                          // reset various flags
	WDTCR |= 0b00011000;               // see docs, set WDCE, WDE
	WDTCR =  0b01000000 | interval;    // set WDIE, and appropriate delay

	wdt_reset();
	set_sleep_mode (SLEEP_MODE_PWR_DOWN);  
	sleep_mode();            // now goes to Sleep and waits for the interrupt
} 


void PlantSystem::sleep(){

	togglePump(false);

	pinMode(pin_sensor_input, OUTPUT);
	digitalWrite(pin_sensor_input, LOW);
	pinMode(pin_config_button, OUTPUT);
	digitalWrite(pin_config_button, LOW);


	int cycles;
	for( cycles = 0; cycles < num_sleep_cycles; ++cycles )
		enableWatchdog(0b100001);

	// restart
	ini();

}

void PlantSystem::loop(){

	const long ms = millis();

	if( is_debug_mode ){
		delay(1000);
		return;
	}


	// Stop the pump if started and reached the end of the cycle
	if( pump_started && pump_started+water_duration < ms ){
		togglePump(false);
		// pump finished, sleep again
		sleep();
		return;
	}

	// Pump is not running, take a reading
	if( !pump_started ){

		digitalWrite(pin_sensor_output, HIGH);
		delay(1);
		const bool reading = digitalRead(pin_sensor_input);
		digitalWrite(pin_sensor_output, LOW);

		#ifndef ATTINY
			Serial.printf("Reading: %i\n", reading);
		#endif

		// It's dry, start watering
		if( reading == dry_on )
			waterPlants();	
		// It's not dry, sleep
		else
			sleep();

	}

	
	
}


extern PlantSystem pSystem = PlantSystem();