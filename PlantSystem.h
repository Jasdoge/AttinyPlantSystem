#ifndef PlantSystem_h
#define PlantSystem_h

// ESP32
/*
#define PIN_PUMP 12
#define PIN_SENSOR_INPUT 13
#define PIN_SENSOR_OUTPUT 16
*/

// Attiny85
#define PIN_PUMP 4
#define PIN_SENSOR_OUTPUT 1
#define PIN_SENSOR_INPUT 0
//#define PIN_DEBUG_LED 4

#include <arduino.h>
class PlantSystem{

	public:
		PlantSystem();
		void ini();
		void loop();
		void waterPlants();
		void togglePump( bool on );
		void sleep();
		void enableWatchdog(const byte interval);
		
		// Configuration
		int num_sleep_cycles = 75;			// Sleep cycles at 8sec each. 450 = 1h, 75 = 10 min
		int water_duration = 6000;			// How long to pump water in ms.

		uint8_t pin_pump = PIN_PUMP;
		uint8_t pin_sensor_input = PIN_SENSOR_INPUT;
		uint8_t pin_sensor_output = PIN_SENSOR_OUTPUT;
		//uint8_t pin_debug_led = PIN_DEBUG_LED;
	private:
		long pump_started = 0;				// Time when pump was started
		bool dry_on = HIGH;					// When should the plant be watered?

};

extern PlantSystem pSystem;

#endif