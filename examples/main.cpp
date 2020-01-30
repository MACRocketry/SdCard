#include "Arduino.h"
#include "SdCard.h"

SdCard *sd;

void setup()
{
	Serial.begin(9600);
	while (!Serial); // wait for Serial monitor to start

	if (NULL == (sd = SdCard::ConnectSdCard(0, 10))) {
		while (true) {
			Serial.print("Fail to connect SD Card\n");
			delay(500);
		}
	}
	sd->openNextFile();
}

int cnt = 0;
char str[64];
void loop()
{
	int bytes = sprintf(str, "Hello World: %d\n", cnt++);
	Serial.print(str);
	sd->write(str, bytes);

	if (0 == cnt % 100) {
		sd->openNextFile();
	}
	delay(50);
}
