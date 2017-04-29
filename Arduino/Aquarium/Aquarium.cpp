// Do not remove the include below

#include "Aquarium.h"
#include <HardwareSerial.h>
#include <IPAddress.h>
#include <OneWire.h>

Timer oneWireTimer;
//Timer resetArduinoTriggerTimer;

//int resetCounter = 0;

void oneWireTriggerFunction() {
	oneWireTrigger(&sensors, &mqttClient, &mode, &desiredTemperature);
}

void mqttCallbackFunction(char* topic, byte* payload, unsigned int length) {
	mqttCallback(topic, payload, length, &mode, &desiredTemperature, &mqttClient);
}

//The setup function is called once at startup of the sketch
void setup() {
	desiredTemperature = AQ_DESIRED_TEMPERATURE_DEFAULT;
	mode = AUTO;

	digitalWrite(RESET, HIGH);
	Serial.begin(115200);

	pinMode(AQ_LIGHTS1_PIN, OUTPUT);      // sets the digital pin as output
	pinMode(AQ_HEATER1_PIN, OUTPUT);      // sets the digital pin as output
	pinMode(AQ_COOLER1_PIN, OUTPUT);      // sets the digital pin as output

	gpio(AQ_LIGHTS1_PIN, LOW, AQ_LIGHTS_CB_TOPIC, &mqttClient);   // Initialize digital AQ_LIGHTS1_PIN with OFF
	gpio(AQ_HEATER1_PIN, LOW, AQ_HEATER1_CB_TOPIC, &mqttClient);   // Initialize digital AQ_HEATER1_PIN with OFF
	gpio(AQ_COOLER1_PIN, LOW, AQ_COOLER1_CB_TOPIC, &mqttClient);   // Initialize digital AQ_COOLER1_PIN with OFF

	Ethernet.begin(mac, myIP);

	sensors.begin();  //start the one-wire library
	delay(15);

	mqttConnect(&mqttClient);
	delay(15);

	// Timer callbacks creation
	oneWireTimer.every(OW_REPORTING_INTERVAL, oneWireTriggerFunction);

	//	Reset_Timer.every(RESET_INTERVAL, ResetFunction);
	oneWireTriggerFunction();
}

// The loop function is called in an endless loop
void loop() {
	oneWireTimer.update();
//	resetArduinoTriggerTimer.update();
	if (mqttConnect(&mqttClient)) {
		mqttClient.loop();
	}
}
