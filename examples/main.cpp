#include "Arduino.h"
#include "SdCard.h"

SdCard *sd;

void setup()
{
	sd = SdCard::ConnectSdCard(0, 10);
	Serial.begin(9600);
	delay(100);
}

int cnt = 0;
char str[64];
void loop()
{
	int bytes = sprintf(str, "Hello World, %d\n", cnt++);
	Serial.println(str);
	sd->write(str, bytes);

	delay(1000);
}
