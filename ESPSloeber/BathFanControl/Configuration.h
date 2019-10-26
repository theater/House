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

class Configuration {
	public:
		static const unsigned MQTT_TOPICS_LENGTH = 60;

		bool isSimulated = true;

		String ssid = "KLP-IoT";
		String ssidPassword = "#m@6ink@#";

		IPAddress mqttServerAddress = IPAddress( {
				192, 168, 254, 40 });
		int mqttPort = 1883;
		String mqttClientName = "ESP_BATH_FLOOR1";
		String fanSpeedMqttTopic = "home/floor1/bathroom/fan/speed";
		String humidityMqttTopic = "home/floor1/bathroom/humidity/1";
		String temperatureMqttTopic = "home/floor1/bathroom/temperature/1";
		String modeMqttTopic = "home/floor1/bathroom/mode";
		String desiredHumidityMqttTopic = "home/floor1/bathroom/humidity/desired";

		int mode = 2;
		int desiredHumidity = 60;
		int lowSpeedTreshold = 70;
		int highSpeedTreshold = 80;

		unsigned sensorsUpdateReocurrenceIntervalMillis = 30000;

		Configuration();
		virtual ~Configuration();
		void print();

};

#endif /* CONFIGURATION_H_ */
