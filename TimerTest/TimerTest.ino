/*
	Tests sleep mode
*/
#include <Arduino.h>
#include <avr/sleep.h>
#include <avr/wdt.h>


#define adc_disable() ADCSRA &= ~ bit(ADEN) // disable ADC (before power-off)
#define adc_enable()  (ADCSRA |=  (1<<ADEN)) // re-enable ADC

#define LED_PIN PB1

#define SLEEP_1S 0b000110
#define SLEEP_2S 0b000111
#define SLEEP_4S 0b100000
#define SLEEP_8S 0b100001

//  1 second:  0b000110
//  2 seconds: 0b000111
//  4 seconds: 0b100000
//  8 seconds: 0b100001

// Captures wakeup
ISR(WDT_vect) {
	wdt_disable();  // disable watchdog
}

// Sleep mode
void enableWatchdog(){  
	adc_disable();
	MCUSR = 0;                          // reset various flags
	WDTCR |= 0b00011000;               // see docs, set WDCE, WDE
	WDTCR =  0b01000000 | SLEEP_8S;    // set WDIE, and 4s delay
	wdt_reset();
	set_sleep_mode (SLEEP_MODE_PWR_DOWN);  
	sleep_mode();            // now goes to Sleep and waits for the interrupt
	adc_enable();
} 

void doSleep(){
	delay(100);
	enableWatchdog();
}



// the setup routine runs once when you press reset:
void setup() {  

	pinMode(LED_PIN, OUTPUT);
	digitalWrite(LED_PIN, HIGH);
	delay(100);
	digitalWrite(LED_PIN, LOW);
	delay(100);
	digitalWrite(LED_PIN, HIGH);
	delay(100);
	digitalWrite(LED_PIN, LOW);
	delay(100);

	
}

// the loop routine runs over and over again forever:
void loop() {

	digitalWrite(LED_PIN, HIGH);
	delay(1000);
	digitalWrite(LED_PIN, LOW);
	doSleep();

}



