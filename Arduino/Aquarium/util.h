/*
 * util.h
 *
 *  Created on: 24.04.2017 ã.
 *      Author: theater
 */

#ifndef UTIL_H_
#define UTIL_H_
#include "PubSubClient.h"
#include <DallasTemperature.h>
#include "userData.h"

enum Mode {
	OFF, MANUAL, AUTO
};

void oneWireTrigger(DallasTemperature *sensors, PubSubClient *mqttClient, Mode *mode, byte *desiredTemperature);

void mqttCallback(char* topic, byte* payload, unsigned int length, Mode *mode, byte *desiredTemperature, PubSubClient *mqttClient);
void publishTemperatureToMQTT(float waterTemperature, const char* topic, char strConvert[10], PubSubClient* mqttClient);
int mqttConnect(PubSubClient *mqttClient);
void mqttTopicHandler(String topic, String payload, Mode *mode, byte *desiredTemperature, PubSubClient *mqttClient);
void pinCommandHandler(int pin, const char* callbackTopic, String payload,  Mode mode, PubSubClient *mqttClient);

void aquariumTriggerLogics(float waterTemperature, Mode mode, byte desiredTemperature, PubSubClient *mqttClient);
void gpio(int dPin, boolean state, const char* callbackTopic, PubSubClient *mqttClient);

Mode modeHandler(String payload);
void oneWireTrigger(DallasTemperature *sensors, PubSubClient *mqttClient, Mode *mode, byte *desiredTemperature);


#endif /* UTIL_H_ */
