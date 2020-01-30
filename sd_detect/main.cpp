#include "Arduino.h"
#include "SD.h"

// test code to detect if SD circuitry was wired correcly

void setup()
{
	Serial.begin(9600);
	while (!Serial);
	
	bool led_on = true;
	bool ret = SD.begin(10);

	// note: LED_BUILTIN shares the same pin for one of the SPI 0 line,
	//       so SD.begin will set pinMode() for LED_BUILTIN. Since we
	//       are not using SD anymore, we can overwrite pinMode()
	pinMode(LED_BUILTIN, OUTPUT);
	while (true) {
		if (ret) Serial.print("Init SD success!\n");
		else     Serial.print("Fail to init SD\n");
		digitalWrite(LED_BUILTIN, led_on = !led_on);
		delay(500);
	}	
}

void loop() {}
