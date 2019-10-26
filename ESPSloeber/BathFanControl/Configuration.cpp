/*
 * Configuration.cpp
 *
 *  Created on: Oct 25, 2019
 *      Author: theater
 */

#include "Configuration.h"

Configuration::Configuration() {
}

Configuration::~Configuration() {
	// TODO Auto-generated destructor stub
}

void Configuration::print() {
	Serial.printf("Current mode set to %d \n", mode);
	//TODO add here better serial printing of all values
}

void Configuration::updateValue(String * configProperty, String value, String parameterName) {
	if (!value.equals("") && !value.equals("********")) {
		*configProperty = value;
		Serial.printf("Parameter %s updated to: %s\n", parameterName.c_str(), value.c_str());
	} else {
		Serial.printf("Received empty response for parameter %s. Will skip update.\n", parameterName.c_str());
	}
}

void Configuration::updateValue(int * configProperty, String value, String parameterName) {
	if (!value.equals("") && !value.equals("********")) {
		*configProperty = value.toInt();
		Serial.printf("Parameter %s updated to: %s\n", parameterName.c_str(), value.c_str());
	} else {
		Serial.printf("Received empty response for parameter %s. Will skip update.\n", parameterName.c_str());
	}
}

void Configuration::updateValue(bool * configProperty, String value, String parameterName) {
	if (value.equals("on")) {
		*configProperty = true;
	} else {
		*configProperty = false;
	}
	Serial.printf("Parameter %s updated to: %s\n", parameterName.c_str(), *configProperty ? "true" : "false");
}

void Configuration::loadEprom() {
	Serial.printf("Loading config from EPROM...");
	EEPROM.begin(512);
	EEPROM.get(0, *this);
	print();
}

void Configuration::saveEprom() {
	EEPROM.begin(512);
	EEPROM.put(0, this);
	EEPROM.commit();
}
