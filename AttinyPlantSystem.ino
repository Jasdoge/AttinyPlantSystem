/*
	Note: When using DIY ATtiny, millis must be enabled
*/
#include <avr/sleep.h>
#include <avr/wdt.h>

#define SLEEP_1S 0b000110
#define SLEEP_2S 0b000111
#define SLEEP_4S 0b100000
#define SLEEP_8S 0b100001

#define pin_pump 0
#define pin_sensor_output 1
#define pin_sensor_input 4
#define pin_debug_led 3
#define pin_config_button 2

#define water_duration 10000
#define num_sleep_cycles 400	// 8 sec each, 450 = 1h

#define dry_on HIGH

#define adc_disable() ADCSRA &= ~ bit(ADEN) // disable ADC (before power-off)
#define adc_enable()  (ADCSRA |=  (1<<ADEN)) // re-enable ADC

#define BLINKS_SLEEP 1
#define BLINKS_LIVE 2
#define BLINKS_PUMP_ON 3
#define BLINKS_DEBUG 5


// Configuration
bool is_debug_mode = false;
//unsigned long pump_started = 0;				// Time when pump was started


// Captures wakeup
ISR(WDT_vect) {
	wdt_disable();  // disable watchdog
}

void enableWatchdog(){
	MCUSR = 0;                          // reset various flags
	WDTCR |= 0b00011000;               // see docs, set WDCE, WDE
	WDTCR =  0b01000000 | SLEEP_8S;    			// set WDIE, and 8s delay
	wdt_reset();
	set_sleep_mode (SLEEP_MODE_PWR_DOWN);  
	sleep_mode();            // now goes to Sleep and waits for the interrupt
} 

void psBlink( byte times ){

	for( byte i=0; i<times; ++i ){

		digitalWrite(pin_debug_led, LOW);
		delay(250);
		digitalWrite(pin_debug_led, HIGH);
		delay(250);

	}

}


void psInit(){

	adc_disable();	// ADC pulls a lot of current, without it, we get down to ~1uA while sleeping
	pinMode(pin_pump, OUTPUT);
	pinMode(pin_sensor_output, OUTPUT);
	pinMode(pin_sensor_input, INPUT);
	pinMode(pin_config_button, INPUT_PULLUP);
	pinMode(pin_debug_led, OUTPUT);
	digitalWrite(pin_pump, LOW);
	digitalWrite(pin_sensor_output, LOW);
	digitalWrite(pin_debug_led, HIGH);		// Connected on cathode
	is_debug_mode = digitalRead(pin_config_button) == LOW;

	byte flashes = is_debug_mode ? BLINKS_DEBUG : BLINKS_LIVE;
	digitalWrite(pin_sensor_output, HIGH);
	psBlink(flashes);

}

// enables sleep mode
void psSleep(){

	togglePump(false);

	psBlink(BLINKS_SLEEP);

	pinMode(pin_sensor_input, OUTPUT);
	digitalWrite(pin_sensor_input, LOW);
	pinMode(pin_config_button, OUTPUT);
	digitalWrite(pin_config_button, LOW);

	for( int cycles = 0; cycles < num_sleep_cycles; ++cycles )
		enableWatchdog();

	// restart
	psInit();

}

void togglePump( bool on ){

	if( on ){

		psBlink(BLINKS_PUMP_ON);
		// Fade in over ~0.5 sec
		for( byte i=0; i<=250; i=i+2 ){
			analogWrite(pin_pump, i);
			delay(5);
		}
		digitalWrite(pin_pump, HIGH);
		delay(water_duration);
		psSleep();

	}
	else
		digitalWrite(pin_pump, LOW);
	

}

// Reads the water level
void psCheck(){

	digitalWrite(pin_sensor_output, HIGH);
	delay(50);
	const bool reading = digitalRead(pin_sensor_input);
	delay(1);
	digitalWrite(pin_sensor_output, LOW);

	// It's dry, start watering
	if( reading == dry_on )
		togglePump(true);
	// It's not dry, sleep
	else
		psSleep();

}












void setup(){
	
	psInit();

}
void loop(){
	if( !is_debug_mode )
		psCheck();
}
