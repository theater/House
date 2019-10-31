// Do not remove the include below

#include "BathFanControl.h"

#include <DHT.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <HardwareSerial.h>
#include <IPAddress.h>
#include <PubSubClient.h>
#include <stdlib_noniso.h>
#include <Timer.h>
#include <WiFiClient.h>
#include <cstdint>
#include <cstring>

#include "HttpHandler.h"
#include "Util.h"
#include "Configuration.h"

// Constants
const int DHT_PIN = 4;
const byte LOW_SPEED = 0;
const byte HIGH_SPEED = 1;


// Configuration
Configuration configData;


// Variables
float humiditySensorValue = 60;
float temperatureSensorValue = 20;
uint32_t interruptTime = 0;

// Objects
Pin *fanSpeedPin;
WiFiClient wifiClient;
PubSubClient *mqttClient;
DHT sensor(DHT_PIN, DHT22);
HttpHandler *httpHandler;

// Timers
Timer sensorsUpdateTrigger;

void initializeGpioPinModes() {
	fanSpeedPin = new Pin(2, configData.fanSpeedMqttTopic.c_str());
	pinMode(fanSpeedPin->getPin(), OUTPUT);
	pinMode(DHT_PIN, INPUT_PULLUP);
}

void loopUntilConnected() {
	while (!wifiConnect(configData.ssid.c_str(), configData.ssidPassword.c_str())) {
		// Loop until connected
	}
	String ipAddress = WiFi.localIP().toString();
	Serial.printf("IP received = %s", ipAddress.c_str());

}

void mqttConnnect() {
	mqttClient = new PubSubClient(configData.mqttServerAddress, 1883, mqttCallback, wifiClient);

	const int maxRetries = 5;
	int counter = 0;
	const char *CLIENT_NAME = configData.mqttClientName.c_str();
	while (!mqttClient->connected() && counter < maxRetries) {
		Serial.println("Connecting to MQTT...");
		if (mqttClient->connect(CLIENT_NAME, "user", "password")) {
			Serial.println("MQTT connected sucessfully.");
		} else {
			Serial.printf("MQTT connect failed with state=%d\n", mqttClient->state());
			delay(5000);
		}
		counter++;
	}
	if (counter != maxRetries) {
		mqttClient->publish(configData.mqttClientName.c_str(), (String("Hello from ") + CLIENT_NAME).c_str());
	} else {
		Serial.println("Error connecting to MQTT server. Giving up.");
	}
}

void subscribeToMqttTopics() {
	mqttClient->subscribe(configData.modeMqttTopic.c_str());
	mqttClient->subscribe(configData.desiredHumidityMqttTopic.c_str());
}

void setup() {
	delay(1000);
	Serial.begin(115200);

	configData.loadEprom();

	loopUntilConnected();

	httpHandler = new HttpHandler(&configData);
	httpHandler->init();
	mqttConnnect();
	subscribeToMqttTopics();

	initializeGpioPinModes();
	sensor.begin();
	sensorsUpdateTrigger.every(configData.sensorsUpdateReocurrenceIntervalMillis, &timerUpdate);

	timerUpdate();
}

void timerUpdate() {
	humiditySensorValue = retrieveHumidity();
	temperatureSensorValue = retrieveTemperature();
	controlHumidity(humiditySensorValue);
}

int retrieveHumidity() {
	float value = configData.isSimulated ? random(40, 100) : sensor.readHumidity();
	if (isnan(value)) {
		Serial.println(PROBLEM_READING_SENSOR_VALUE);
		return -999;
	}
	int result = (int) value;
	Serial.printf("Humidity sensor value: %d \n", result);
	publishValueMqtt(result, configData.humidityMqttTopic.c_str());
	return result;
}

float retrieveTemperature() {
	float value = configData.isSimulated ? random(15, 30) : sensor.readTemperature();
	if (isnan(value)) {
		Serial.println(PROBLEM_READING_SENSOR_VALUE);
		return -999;
	}

	Serial.print("Temperature sensor value:");
	Serial.println(value);
	publishValueMqtt(value, configData.temperatureMqttTopic.c_str());
	return value;
}

void publishValueMqtt(double value, const char* topic) {
	char strValue[10];
	dtostrf(value, 3, 2, strValue);
	mqttClient->publish(topic, strValue);
}

void controlHumidity(int humidityValue) {
	Serial.printf("Measured humidity value: %d \n", humidityValue);
	if (humidityValue > configData.desiredHumidity) {
		if (humidityValue > configData.highSpeedThreshold) {
			updatePinState(fanSpeedPin, HIGH_SPEED);
		} else {
			updatePinState(fanSpeedPin, LOW_SPEED);
		}
	} else {
		updatePinState(fanSpeedPin, LOW_SPEED);
	}
	Serial.printf("Fan speed pin state = %d\n", digitalRead(fanSpeedPin->getPin()));
}

void updatePinState(Pin *pin, const byte state) {
	digitalWrite(pin->getPin(), state);
	Serial.print("Updated pin ");
	Serial.print(pin->getPin());
	Serial.print(" to ");
	Serial.println(state);
	mqttClient->publish(pin->getTopic(), state ? "ON" : "OFF");
}

void mqttCallback(const char* topic, const byte* payload, const unsigned int length) {
	Serial.println("Received update via MQTT ! ! !");
	char cPayload[10];
	for (unsigned int i = 0; i <= length; i++) {
		cPayload[i] = (char) payload[i];
	}
	cPayload[length] = '\0';
	Serial.printf("Received message: topic=%s, payload=%s\n", topic, cPayload);

	if (strcmp(topic, configData.modeMqttTopic.c_str()) == 0) {
		configData.mode = atoi(cPayload);
		Serial.printf("Configuration to be updated. Mode=%d\n",configData.mode);
	}
}

void loop() {
	httpHandler->handleClient();
	sensorsUpdateTrigger.update();
	mqttClient->loop();
}
