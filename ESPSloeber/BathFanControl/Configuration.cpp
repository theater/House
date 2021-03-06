/*
 * Configuration.cpp
 *
 *  Created on: Oct 25, 2019
 *      Author: theater
 */

#include "Configuration.h"

void Configuration::print() {
	Serial.println("###########################################");
	String simulated = isSimulated ? "true" : "false";
	Serial.printf("Simulated=%s \n", simulated.c_str());
	Serial.printf("Configuration serial number=%d \n", serialNumber);
	Serial.println("###########################################");
	Serial.printf("SSID=%s \n", ssid.c_str());
	Serial.printf("SSID Password=%s \n", ssidPassword.c_str());
	Serial.println("###########################################");
	Serial.printf("MQTT Server/port=%s:%d \n", mqttServerAddress.toString().c_str(), mqttPort);
	Serial.printf("MQTT Client name=%s \n", mqttClientName.c_str());
	Serial.printf("Fan speed topic=%s \n", fanSpeedMqttTopic.c_str());
	Serial.printf("Mirror heating topic=%s \n", mirrorHeatingMqttTopic.c_str());
	Serial.printf("Humidity topic=%s \n", humidityMqttTopic.c_str());
	Serial.printf("Temperature topic=%s \n", temperatureMqttTopic.c_str());
	Serial.printf("Mode topic=%s \n", modeMqttTopic.c_str());
	Serial.printf("Desired humidity topic=%s \n", desiredHumidityMqttTopic.c_str());
	Serial.println("###########################################");
	Serial.printf("Current mode=%d \n", mode);
	Serial.printf("Desired humidity=%d%% \n", desiredHumidity);
	Serial.printf("High speed humidity threshold=%d%% \n", highSpeedThresholdDelta);
	Serial.printf("Humidity threshold tolerance=%d \n", humidityTolerance);
	Serial.printf("Sensors refresh interval=%dms \n", sensorsUpdateReocurrenceIntervalMillis);
	Serial.printf("Temperature correction=%d \n", temperatureCorrection);
	Serial.printf("Humidity Correction=%d \n", humidityCorrection);
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
		Serial.printf("Parameter %s updated to: %d\n", parameterName.c_str(), *configProperty);
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
	int nextAddress = STARTING_ADDRESS;
	Serial.printf("Loading config from EPROM...\nStarting with address=%d. Serial number before load=%d\n", nextAddress, serialNumber);
	EEPROM.begin(512);
	int memSerialNumber;
	memSerialNumber = EEPROM.get(nextAddress, memSerialNumber);
	Serial.printf("Loaded serial number from EPROM=%d\n", memSerialNumber);
	if (memSerialNumber == serialNumber) {
		nextAddress+=sizeof(memSerialNumber);
		ssid = getStringEprom(&nextAddress);
		ssidPassword = getStringEprom(&nextAddress);
		mqttServerAddress.fromString(getStringEprom(&nextAddress));
		mqttClientName = getStringEprom(&nextAddress);
		fanSpeedMqttTopic = getStringEprom(&nextAddress);
		mirrorHeatingMqttTopic = getStringEprom(&nextAddress);
		humidityMqttTopic = getStringEprom(&nextAddress);
		temperatureMqttTopic = getStringEprom(&nextAddress);
		modeMqttTopic = getStringEprom(&nextAddress);
		desiredHumidityMqttTopic = getStringEprom(&nextAddress);
		Serial.printf("Finished loading strings. Last address=%d\n", nextAddress);

		isSimulated = EEPROM.get(nextAddress += sizeof(isSimulated), isSimulated);
		mode = EEPROM.get(nextAddress += sizeof(mode), mode);
		mqttPort = EEPROM.get(nextAddress += sizeof(mqttPort), mqttPort);
		desiredHumidity = EEPROM.get(nextAddress += sizeof(desiredHumidity), desiredHumidity);
		highSpeedThresholdDelta = EEPROM.get(nextAddress += sizeof(highSpeedThresholdDelta), highSpeedThresholdDelta);
		sensorsUpdateReocurrenceIntervalMillis = EEPROM.get(nextAddress += sizeof(sensorsUpdateReocurrenceIntervalMillis),
				sensorsUpdateReocurrenceIntervalMillis);
		temperatureCorrection = EEPROM.get(nextAddress += sizeof(temperatureCorrection), temperatureCorrection);
		humidityCorrection = EEPROM.get(nextAddress += sizeof(humidityCorrection), humidityCorrection);
		humidityTolerance = EEPROM.get(nextAddress += sizeof(humidityTolerance), humidityTolerance);
		Serial.printf("Finished loading primitives. Last address=%d\n", nextAddress);
	} else {
		Serial.printf("Configuration serial number from EPROM not equal to expected. Default values will be used. Value expected=%d, Value in EPROM=%d\n", this->serialNumber, memSerialNumber);
	}

	print();
}

void Configuration::saveEprom() {
	int nextAddress = STARTING_ADDRESS;

	Serial.printf("Saving config from EPROM...\nStarting with address=%d.\n", nextAddress);
	EEPROM.begin(512);

	Serial.printf("Before writing configuration serial number. Address=%d, serial number=%d",nextAddress, serialNumber);
	EEPROM.put(nextAddress, serialNumber);
	nextAddress+=sizeof(serialNumber);

	putStringEprom(&nextAddress, ssid);
	putStringEprom(&nextAddress, ssidPassword);
	putStringEprom(&nextAddress, mqttServerAddress.toString());
	putStringEprom(&nextAddress, mqttClientName);
	putStringEprom(&nextAddress, fanSpeedMqttTopic);
	putStringEprom(&nextAddress, mirrorHeatingMqttTopic);
	putStringEprom(&nextAddress, humidityMqttTopic);
	putStringEprom(&nextAddress, temperatureMqttTopic);
	putStringEprom(&nextAddress, modeMqttTopic);
	putStringEprom(&nextAddress, desiredHumidityMqttTopic);
	Serial.printf("Finished saving strings. Last address=%d\n", nextAddress);

	EEPROM.put(nextAddress += sizeof(isSimulated), isSimulated);
	EEPROM.put(nextAddress += sizeof(mode), mode);
	EEPROM.put(nextAddress += sizeof(mqttPort), mqttPort);
	EEPROM.put(nextAddress += sizeof(desiredHumidity), desiredHumidity);
	EEPROM.put(nextAddress += sizeof(highSpeedThresholdDelta), highSpeedThresholdDelta);
	EEPROM.put(nextAddress += sizeof(sensorsUpdateReocurrenceIntervalMillis), sensorsUpdateReocurrenceIntervalMillis);
	EEPROM.put(nextAddress += sizeof(temperatureCorrection), temperatureCorrection);
	EEPROM.put(nextAddress += sizeof(humidityCorrection), humidityCorrection);
	EEPROM.put(nextAddress += sizeof(humidityTolerance), humidityTolerance);
	Serial.printf("Finished saving primitives. Last address=%d\n", nextAddress);
	EEPROM.commit();
	Serial.printf("EPROM used=%d bytes\n", nextAddress);
}

void Configuration::putStringEprom(int *address, String property) {
	int length = property.length();
	Serial.printf("Property %s / length=%d\n", property.c_str(), length);
	EEPROM.put(*address, length);

	*address += sizeof(length);
	for (int i = 0; i < length; i++) {
		EEPROM.write(*address + i, property[i]);
	}
	EEPROM.write(*address + length, '\0');
	*address += length + 1;
	Serial.printf("Next address = %d\n", *address);
}

String Configuration::getStringEprom(int *address) {
	int length = EEPROM.get(*address, length);
	*address += sizeof(length);

	Serial.printf("Property length=%d, next address=%d\n", length, *address);
	char tempArr[100];
	Serial.print("Starting EEPROM read. Characters=");
	unsigned char tempChar;
	for (int i = 0; i < length; i++) {
		tempChar = EEPROM.read(*address + i);
		tempArr[i] = tempChar;
		Serial.printf("%c", tempArr[i]);
	}
	tempArr[length] = '\0';
	*address += length + 1;

	return String(tempArr);
}

