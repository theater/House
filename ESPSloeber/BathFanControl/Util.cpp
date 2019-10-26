/*
 * Util.cpp
 *
 *  Created on: Feb 23, 2019
 *      Author: theater
 */

#include <ESP8266WiFi.h>
#include <include/wl_definitions.h>

#include "C:/Eclipse/sloeber/arduinoPlugin/packages/arduino/hardware/avr/1.6.20/cores/arduino/Arduino.h"
#include "C:/Eclipse/sloeber/arduinoPlugin/packages/arduino/hardware/avr/1.6.20/cores/arduino/HardwareSerial.h"
#include "C:/Eclipse/sloeber/arduinoPlugin/packages/arduino/hardware/avr/1.6.20/cores/arduino/IPAddress.h"

bool wifiConnect(const char* ssid, const char* password) {
	int attempts = 0;
	WiFi.begin(ssid, password);
	Serial.println();
	Serial.print("Connecting");
	while (WiFi.status() != WL_CONNECTED) {
		delay(500);
		Serial.print(".");
		if (attempts++ == 30) {
			Serial.println("\nConect not possible. Will retry in 5 sec.");
			WiFi.disconnect(true);
			delay(5000);
			return false;
		}
	}
	Serial.println("success!");
	Serial.print("IP Address is: ");
	Serial.println(WiFi.localIP());
	return true;
}

bool handleHtmlCheckBox(String value) {
	return value.equals("on") ? true : false;
}

String booleanToCheckBoxResponse(bool value) {
	return value ? "1" : "0";
}
