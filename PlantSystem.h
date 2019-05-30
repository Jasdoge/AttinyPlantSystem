#ifndef PlantSystem_h
#define PlantSystem_h
#include <Arduino.h>

// Attiny85
#define PIN_PUMP 0
#define PIN_SENSOR_OUTPUT 1
#define PIN_SENSOR_INPUT 4
#define PIN_DEBUG_LED 3
#define PIN_CONFIG_BUTTON 2



class PlantSystem{

	public:
		PlantSystem();
		void ini();
		void loop();
		void togglePump( bool on );
		void sleep();
		void enableWatchdog();
		
		// Configuration
		const byte num_sleep_cycles = 75;			// Sleep cycles at 8sec each. 450 = 1h, 75 = 10 min
		const int water_duration = 10000;			// How long to pump water in ms.	
		bool is_debug_mode = false;

		const uint8_t pin_pump = PIN_PUMP;
		const uint8_t pin_sensor_input = PIN_SENSOR_INPUT;
		const uint8_t pin_sensor_output = PIN_SENSOR_OUTPUT;
		const uint8_t pin_debug_led = PIN_DEBUG_LED;
		const uint8_t pin_config_button = PIN_CONFIG_BUTTON;
	private:
		long pump_started = 0;				// Time when pump was started
		const bool dry_on = HIGH;					// When should the plant be watered?

};

#endif