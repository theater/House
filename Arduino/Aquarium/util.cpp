/*
 * util.c
 *
 *  Created on: 24.04.2017 ã.
 *      Author: theater
 */
#include "util.h"
#include "userData.h"
#include <PubSubClient.h>

int mqttConnect(PubSubClient *mqttClient) {
	if (!mqttClient->connected()) {
		if (mqttClient->connect("ArduinoNANO-AQ", "mqttuser", "MqTtUser")) {
			mqttClient->publish("Arduino", "Arduino-AQ is UP");

			mqttClient->subscribe(AQ_MODE_TOPIC);
			mqttClient->subscribe(AQ_DESIRED_TEMPERATURE_TOPIC);

			mqttClient->subscribe(AQ_LIGHTS_TOPIC);
			mqttClient->subscribe(AQ_HEATER1_TOPIC);
			mqttClient->subscribe(AQ_COOLER1_TOPIC);

//      Serial.write("Connected to MQTT\n");
			return 1;
		} else {
//       Serial.write("Error connecting to MQTT\n");
			return 0;
		}
	} else
		return 1;
}

void oneWireTrigger(DallasTemperature *sensors, PubSubClient *mqttClient, Mode *mode, float *desiredTemperature) {
	//  Start reading sensors...
	sensors->requestTemperatures(); // Send the command to get temperatures from 1-wire
	char strConvert[10];
	// 1-wire HERE
	delay(10);

	float waterTemperature = sensors->getTempCByIndex(0);
	publishTemperatureToMQTT(waterTemperature, AQ_WATER_TEMP_SENSOR1_TOPIC, strConvert, mqttClient);

	float outsideTemperature = sensors->getTempCByIndex(1);
	publishTemperatureToMQTT(outsideTemperature, AQ_OUTSIDE_SENSOR1_TOPIC, strConvert, mqttClient);

	aquariumTriggerLogics(waterTemperature, *mode, *desiredTemperature, mqttClient);
}

void publishTemperatureToMQTT(float temperature, const char* topic, char strConvert[10], PubSubClient* mqttClient) {
	if (temperature > -20) {  	// sample check not to send -999 if sensor is not read OK
		dtostrf(temperature, 6, 3, strConvert);
		mqttClient->publish(topic, strConvert);
	}
}

void aquariumTriggerLogics(float waterTemperature, Mode mode, float desiredTemperature, PubSubClient *mqttClient) {
	if (mode == AUTO) {
		if (waterTemperature > desiredTemperature + 0.25) {
			gpio(AQ_HEATER1_PIN, LOW, AQ_HEATER1_CB_TOPIC, mqttClient);
			if((waterTemperature > desiredTemperature + 0.5) && ) {
				gpio(AQ_COOLER1_PIN, HIGH, AQ_COOLER1_CB_TOPIC, mqttClient);
			}
		} else if (waterTemperature < desiredTemperature) {
			gpio(AQ_COOLER1_PIN, LOW, AQ_COOLER1_CB_TOPIC, mqttClient);
			if (waterTemperature < desiredTemperature - 0.25) {
				gpio(AQ_HEATER1_PIN, HIGH, AQ_HEATER1_CB_TOPIC, mqttClient);
			}
		}
		delay(10);
	}
}

void mqttCallback(char* topic, byte* payload, unsigned int length, Mode *mode, float *desiredTemperature, PubSubClient *mqttClient) {
// ##### this function is the main part that triggers event based on the MQTT received messages. All the important input stuff is here...
// ####### byte* to String transformation of payload starts here
	char cPayload[10];
	for (int i = 0; i <= length; i++) {
		cPayload[i] = (char) payload[i];
	}
	cPayload[length] = '\0';
// ####### byte* to String transformation ends here

// #### Real work/Logics start here. Using IFs to destinguish for the different MQTT subscriptions/relays (unfortunalte string not allowed in switch operator) :(
	mqttTopicHandler(topic, cPayload, mode, desiredTemperature, mqttClient);
}

void mqttTopicHandler(String topic, const char* payload, Mode *mode, float *desiredTemperature, PubSubClient *mqttClient) {
	if (topic == AQ_LIGHTS_TOPIC) {
		pinCommandHandler(AQ_LIGHTS1_PIN, AQ_LIGHTS_CB_TOPIC, payload, *mode, mqttClient);
	} else if (topic == AQ_HEATER1_TOPIC) {
		pinCommandHandler(AQ_HEATER1_PIN, AQ_HEATER1_CB_TOPIC, payload, *mode, mqttClient);
	} else if (topic == AQ_COOLER1_TOPIC) {
		pinCommandHandler(AQ_COOLER1_PIN, AQ_COOLER1_CB_TOPIC, payload, *mode, mqttClient);
	} else if (topic == AQ_DESIRED_TEMPERATURE_TOPIC) {
		float convertedDesiredTemperature = atof(payload);
		if(convertedDesiredTemperature) {
			*desiredTemperature = convertedDesiredTemperature;
		}
//		Serial.print("Des:");
//		Serial.println(*desiredTemperature);
	} else if (topic == AQ_MODE_TOPIC) {
		*mode = atoi(payload);
	}
}

void pinCommandHandler(int pin, const char* callbackTopic, String payload,  Mode mode, PubSubClient *mqttClient) {
	if (payload == "ON") {
		gpio(pin, HIGH, callbackTopic, mqttClient);
	} else {
		gpio(pin, LOW, callbackTopic, mqttClient);
	}
}

void gpio(int pin, boolean state, const char* callbackTopic, PubSubClient *mqttClient) {
	boolean pinState = digitalRead(pin);
	if (state == HIGH && !pinState) {
		digitalWrite(pin, HIGH);
		mqttClient->publish(callbackTopic,"ON");
	} else if(state == LOW && pinState){
		digitalWrite(pin, LOW);
		mqttClient->publish(callbackTopic,"OFF");
	}
}

//void ResetFunction(PubSubClient *mqttClient, ) {
//	if (!mqttConnect(mqttClient)) {
//		ManualAQLogics(sensors);
//		resetCounter++;
//	} else {
//		resetCounter = 0;
//	}
//	if (resetCounter >= 3) {
//		Enc28J60.init(mac);
//	}
//	if (resetCounter > 5) {
//		gpio(RESET, LOW);
//	}
//}

int freeRam() {
	extern int __heap_start, *__brkval;
	int v;
	return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}
