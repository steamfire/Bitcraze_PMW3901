#include "Bitcraze_PMW3901.h"

// Using digital pin 10 for chip select
Bitcraze_PMW3901 flow(10);

void setup() {
	Serial.begin(9600);

	while (!flow.begin()) 
	{
	Serial.println("Initialization of the flow sensor failed");
	delay(1000);
	}
}

int16_t deltaX,deltaY;

void loop() {
	// Get motion count since last call
	flow.readBurst();

	Serial.print("\tM: ");
	Serial.print(flow.getBurst()->motion);
	Serial.print("\tO: ");
	Serial.print(flow.getBurst()->observation);
	Serial.print("\tX: ");
	Serial.print(flow.getBurst()->delta[0]);
	Serial.print("\tY: ");
	Serial.print(flow.getBurst()->delta[1]);
	Serial.print("\tQ: ");
	Serial.print(flow.getBurst()->squal);
	Serial.print("\tRS: ");
	Serial.print(flow.getBurst()->sum);
	Serial.print("\tRH: ");
	Serial.print(flow.getBurst()->rawMax);
	Serial.print("\tRL: ");
	Serial.print(flow.getBurst()->rawMin);
	Serial.print("\tSH: ");
	Serial.print(flow.getBurst()->shutterUpper);
	Serial.print("\tSL: ");
	Serial.print(flow.getBurst()->shutterLower);
	Serial.println();

	delay(16);
}
