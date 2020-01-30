#include "Arduino.h"
#include "SD.h"

// test code to detect if SD circuitry was wired correcly

void setup()
{
	Serial.begin(9600);
	while (!Serial);
	pinMode(LED_BUILTIN, OUTPUT);
	bool led_on = true;

	bool ret = SD.begin(10);
	while (true) {
		if (ret) Serial.print("Fail to init SD\n");
		else     Serial.print("Init SD success!\n");
		digitalWrite(LED_BUILTIN, led_on = !led_on);
		delay(500);
	}	
}

void loop() {}
