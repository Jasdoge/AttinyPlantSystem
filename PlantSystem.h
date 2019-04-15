#ifndef PlantSystem_h
#define PlantSystem_h
#include <Arduino.h>

// Attiny85
#define PIN_PUMP 4
#define PIN_SENSOR_OUTPUT 1
#define PIN_SENSOR_INPUT 0
#define PIN_DEBUG_LED 3
#define PIN_CONFIG_BUTTON 2



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
		bool is_debug_mode = false;

		uint8_t pin_pump = PIN_PUMP;
		uint8_t pin_sensor_input = PIN_SENSOR_INPUT;
		uint8_t pin_sensor_output = PIN_SENSOR_OUTPUT;
		uint8_t pin_debug_led = PIN_DEBUG_LED;
		uint8_t pin_config_button = PIN_CONFIG_BUTTON;
	private:
		long pump_started = 0;				// Time when pump was started
		bool dry_on = HIGH;					// When should the plant be watered?

};

extern PlantSystem pSystem;

#endif