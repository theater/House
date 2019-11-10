/*
 * Configuration.h
 *
 *  Created on: Oct 25, 2019
 *      Author: theater
 */

#ifndef CONFIGURATION_H_
#define CONFIGURATION_H_

#include <HardwareSerial.h>
#include <IPAddress.h>
#include <EEPROM.h>

class Configuration {
	private:
		static const unsigned STARTING_ADDRESS = 10;
		static const unsigned MQTT_TOPICS_LENGTH = 60;
	public:
		int configurationHash = 462;

		bool isSimulated = true;

		String ssid = "KLP-IoT";
		String ssidPassword = "#m@6ink@#";

		IPAddress mqttServerAddress = IPAddress((uint8_t) 192, (uint8_t) 168, (uint8_t) 254, (uint8_t) 40);
		int mqttPort = 1883;
		String mqttClientName = "ESP_TMP";

		String fanSpeedMqttTopic = "fan123";
		String mirrorHeatingMqttTopic = "mirror123";
		String humidityMqttTopic = "humidity123";
		String temperatureMqttTopic = "temperature123";
		String modeMqttTopic = "mode123";
		String desiredHumidityMqttTopic = "desiredHumidity123";

		int mode = 2;
		int desiredHumidity = 60;
		int highSpeedThresholdDelta = 15;

		int temperatureCorrection = 0;
		int humidityCorrection = 0;
		int humidityTolerance = 2;

		unsigned sensorsUpdateReocurrenceIntervalMillis = 30000;

		void print();
		/**
		 * @param configProperty = the property to be updated
		 * @param value = the value to which the property will be updated
		 * @param parameterName = description of the property for logging
		 */
		void updateValue(String * configProperty, String value, String parameterName);
		void updateValue(int * configProperty, String value, String parameterName);
		void updateValue(bool * configProperty, String value, String parameterName);
		void loadEprom();
		void saveEprom();
	private:
		String getStringEprom(int* address);
		void putStringEprom(int* address, const String);
};

#endif /* CONFIGURATION_H_ */
