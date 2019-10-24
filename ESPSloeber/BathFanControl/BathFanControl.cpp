// Do not remove the include below

#include "BathFanControl.h"

#include <DHT.h>
#include <EEPROM.h>
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

// Constants
const unsigned MQTT_TOPICS_LENGTH = 60;
const int DHT_PIN = 4;
const byte LOW_SPEED = 0;
const byte HIGH_SPEED = 1;

// Configuration
class {
	public:
		bool isSimulated = false;
		char ssid[15] = "KLP";
		char ssidPassword[15] = "charly78";
		IPAddress mqttServerAddress = IPAddress( {
				192, 168, 254, 40 });
		int mqttPort = 1883;
		char mqttClientName[20] = "ESP_BATH_FLOOR1";
		int mode = 2;
		int desiredHumidity = 60;
		int lowSpeedTreshold = 70;
		int highSpeedTreshold = 80;
		char fanSpeedMqttTopic[MQTT_TOPICS_LENGTH] = "home/floor1/bathroom/fan/speed";
		char humidityMqttTopic[MQTT_TOPICS_LENGTH] = "home/floor1/bathroom/humidity/1";
		char temperatureMqttTopic[MQTT_TOPICS_LENGTH] = "home/floor1/bathroom/temperature/1";
		char modeMqttTopic[MQTT_TOPICS_LENGTH] = "home/floor1/bathroom/mode";
		char desiredHumidityMqttTopic[MQTT_TOPICS_LENGTH] = "home/floor1/bathroom/humidity/desired";
		unsigned sensorsUpdateReocurrenceIntervalMillis = 30000;

		void print() {
			Serial.printf("Current mode set to %d \n", mode);
		}
} configData;

const Pin FAN_SPEED_PIN = Pin(2, configData.fanSpeedMqttTopic);

// Variables
float humiditySensorValue = 60;
float temperatureSensorValue = 20;
uint32_t interruptTime = 0;

// Objects
WiFiClient wifiClient;
PubSubClient mqttClient(configData.mqttServerAddress, 1883, mqttCallback, wifiClient);
DHT sensor(DHT_PIN, DHT22);
HttpHandler *httpHandler = new HttpHandler();

// Timers
Timer sensorsUpdateTrigger;

void initializeGpioPinModes() {
	pinMode(FAN_SPEED_PIN.getPin(), OUTPUT);
	pinMode(DHT_PIN, INPUT_PULLUP);
}

void loopUntilConnected() {
	while (!wifiConnect(configData.ssid, configData.ssidPassword)) {
		// Loop until connected
	}
	String ipAddress = WiFi.localIP().toString();
	Serial.printf("IP received = %s", ipAddress.c_str());

	while (!mqttClient.connected()) {
		Serial.println("Connecting to MQTT...");

		if (mqttClient.connect(configData.mqttClientName, "user", "password")) {
			Serial.println("MQTT connected sucessfully");
		} else {
			Serial.print("failed with state ");
			Serial.println(mqttClient.state());
			delay(5000);
		}
	}

	mqttClient.publish(configData.mqttClientName, "Hello from BATHROOM-FLOOR1");
}

void subscribeToMqttTopics() {
	mqttClient.subscribe(configData.modeMqttTopic);
	mqttClient.subscribe(configData.desiredHumidityMqttTopic);
}

void httpServerInitialize() {
	httpHandler->on("/", std::bind(&HttpHandler::handleRootRequest, httpHandler));
	httpHandler->begin();
}

void setup() {
	delay(1000);
	Serial.begin(115200);
	loopUntilConnected();

	loadConfigEPROM();

	subscribeToMqttTopics();

	initializeGpioPinModes();
	sensor.begin();

	sensorsUpdateTrigger.every(configData.sensorsUpdateReocurrenceIntervalMillis, &timerUpdate);

	timerUpdate();

	httpServerInitialize();
}

void timerUpdate() {
	humiditySensorValue = retrieveHumidity();
	temperatureSensorValue = retrieveTemperature();
	controlHumidity(humiditySensorValue);
}

int retrieveHumidity() {
	float value = configData.isSimulated ? random(40, 100) : sensor.readHumidity();
	if (isnan(value)) {
		Serial.println("Problem reading sensor value.");
		return -999;
	}
	int result = (int) value;
	Serial.printf("Humidity sensor value: %d \n", result);
	publishValueMqtt(result, configData.humidityMqttTopic);
	return result;
}

float retrieveTemperature() {
	float value = configData.isSimulated ? random(15, 30) : sensor.readTemperature();
	if (isnan(value)) {
		Serial.println("Problem reading sensor value.");
		return -999;
	}

	Serial.print("Temperature sensor value:");
	Serial.println(value);
	publishValueMqtt(value, configData.temperatureMqttTopic);
	return value;
}

void publishValueMqtt(double value, const char* topic) {
	char strValue[10];
	dtostrf(value, 3, 2, strValue);
	mqttClient.publish(topic, strValue);
}

void controlHumidity(int humidityValue) {
	Serial.printf("Measured humidity value: %d \n", humidityValue);
	if (humidityValue > configData.desiredHumidity) {
		if (humidityValue > configData.highSpeedTreshold) {
			updatePinState(FAN_SPEED_PIN, HIGH_SPEED);
		} else {
			updatePinState(FAN_SPEED_PIN, LOW_SPEED);
		}
	} else {
		updatePinState(FAN_SPEED_PIN, LOW_SPEED);
	}
	Serial.printf("Fan speed pin state = %d\n", digitalRead(FAN_SPEED_PIN.getPin()));
}

void updatePinState(Pin pin, const byte state) {
	digitalWrite(pin.getPin(), state);
	Serial.print("Updated pin ");
	Serial.print(pin.getPin());
	Serial.print(" to ");
	Serial.println(state);
	mqttClient.publish(pin.getTopic(), state ? "ON" : "OFF");
}

void mqttCallback(const char* topic, const byte* payload, const unsigned int length) {
	Serial.println("Received update via MQTT ! ! !");
	char cPayload[10];
	for (unsigned int i = 0; i <= length; i++) {
		cPayload[i] = (char) payload[i];
	}
	cPayload[length] = '\0';
	Serial.printf("Received message: topic=%s, payload=%s\n", topic, cPayload);

	bool configUpdated = false;
	if (strcmp(topic, configData.modeMqttTopic) == 0) {
		configUpdated = true;
		configData.mode = atoi(cPayload);
		Serial.printf("Configuration to be updated. Mode=%d\n",configData.mode);
	}

	if (configUpdated) {
		Serial.println("Writing config to EPROM");
		writeConfigToEPROM();
	}
}

void loadConfigEPROM() {
	Serial.printf("Loading config from EPROM...");
	EEPROM.begin(512);
	EEPROM.get(0, configData);
	configData.print();
}

void writeConfigToEPROM() {
	EEPROM.begin(512);
	EEPROM.put(0, configData);
	EEPROM.commit();
}

void loop() {
	sensorsUpdateTrigger.update();
	mqttClient.loop();
	httpHandler->handleClient();
}
