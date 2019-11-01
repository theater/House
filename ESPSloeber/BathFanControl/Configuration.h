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
	public:
		static const unsigned MQTT_TOPICS_LENGTH = 60;

		bool isSimulated = true;

		String ssid = "KLP-IoT";
		String ssidPassword = "#m@6ink@#";

		IPAddress mqttServerAddress = IPAddress((uint8_t) 192, (uint8_t) 168, (uint8_t) 254, (uint8_t) 40);
		int mqttPort = 1883;
		String mqttClientName = "ESP_TMP";

		String fanSpeedMqttTopic = "home/floor1/bathroom/fan/speed";
		String mirrorHeatingMqttTopic = "home/floor1/bathroom/mirror/1";
		String humidityMqttTopic = "home/floor1/bathroom/humidity/1";
		String temperatureMqttTopic = "home/floor1/bathroom/temperature/1";
		String modeMqttTopic = "home/floor1/bathroom/mode";
		String desiredHumidityMqttTopic = "home/floor1/bathroom/humidity/desired";

		int mode = 2;
		int desiredHumidity = 60;
		int lowSpeedThreshold = 70;
		int highSpeedThreshold = 80;

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
