// Do not remove the include below

#include "BathFanControl.h"

#include <PubSubClient.h>
#include <ESP8266WiFi.h>
#include <DHT.h>
#include <Timer.h>

#include "Util.h"

// Constants
const bool SIMULATED = false;
const char* SSID = "KLP";
const char* PASSWORD = "charly78";
const IPAddress MQTT_SERVER = IPAddress( {
		192, 168, 254, 40 });
const int MQTT_PORT = 1883;
const char* MQTT_CLIENT_NAME = "ESP_BATH_FLOOR1";
const int DESIRED_HUMIDITY = 60;
const int LOW_TRESHOLD_VALUE = 70;
const int HIGH_TRESHOLD_VALUE = 80;

const Pin FAN_POWER_PIN = Pin(0, "home/floor1/bathroom/fan/state");
const Pin FAN_SPEED_PIN = Pin(2, "home/floor1/bathroom/fan/speed");
const int DHT_PIN = 4;
const int SWITCH_PIN = 13;

const char* DHT_HUMIDITY_TOPIC = "home/floor1/bathroom/humidity/1";
const char* DHT_TEMPERATURE_TOPIC = "home/floor1/bathroom/temperature/1";
const char* MODE_TOPIC = "home/floor1/bathroom/mode";

const byte OFF = 0;
const byte ON = 1;

const unsigned REOCCURRENCE = 30000;
const unsigned DEBOUNCE_VALUE = 300;

// Variables
float humiditySensorValue = 60;
float temperatureSensorValue = 20;
uint32_t interruptTime = 0;

// Objects
WiFiClient wifiClient;
//PubSubClient mqttClient(wifiClient);
PubSubClient mqttClient(MQTT_SERVER, 1883, mqttCallback, wifiClient);
DHT sensor(4, DHT22);

// Timers
Timer sensorsUpdateTrigger;

// Functions
void handleInterrupt() {
	if (millis() - interruptTime > DEBOUNCE_VALUE) {
		int pin = FAN_POWER_PIN.getPin();
		detachInterrupt(pin);

		Serial.println("Interrupt");
		int pinState = digitalRead(pin);
		updatePinState(FAN_POWER_PIN, !pinState);


		interruptTime = millis();
		attachInterrupt(digitalPinToInterrupt(SWITCH_PIN), handleInterrupt, CHANGE);
	}
}

void initializeGpioPinModes() {
	pinMode(FAN_POWER_PIN.getPin(), OUTPUT);
	pinMode(FAN_SPEED_PIN.getPin(), OUTPUT);
	pinMode(DHT_PIN, INPUT_PULLUP);
	pinMode(SWITCH_PIN, INPUT_PULLUP);
	attachInterrupt(digitalPinToInterrupt(SWITCH_PIN), handleInterrupt, CHANGE);
}

void loopUntilConnected() {
	while (!wifiConnect(SSID, PASSWORD)) {
	}

	mqttClient.setServer(MQTT_SERVER, MQTT_PORT);
//	mqttClient.setCallback(mqttCallback);

	while (!mqttClient.connected()) {
		Serial.println("Connecting to MQTT...");

		if (mqttClient.connect(MQTT_CLIENT_NAME, "user", "password")) {
			Serial.println("MQTT connected sucessfully");
		} else {
			Serial.print("failed with state ");
			Serial.println(mqttClient.state());
			delay(5000);
		}
	}

	mqttClient.publish(MQTT_CLIENT_NAME, "Hello from BATHROOM-FLOOR1");
}

void setup() {
	delay(1000);
	Serial.begin(115200);
	loopUntilConnected();
	mqttClient.subscribe(MODE_TOPIC);

	initializeGpioPinModes();
	sensor.begin();

	sensorsUpdateTrigger.every(REOCCURRENCE, &timerUpdate);

	timerUpdate();
}

void timerUpdate() {
	humiditySensorValue = retrieveHumidity();
	temperatureSensorValue = retrieveTemperature();
	controlHumidity(humiditySensorValue);
}

int retrieveHumidity() {
	float value = SIMULATED ? random(40, 100) : sensor.readHumidity();
	if (isnan(value)) {
		Serial.println("Problem reading sensor value.");
		return -999;
	}
	int result = (int) value;
	Serial.printf("Humidity sensor value: %d \n", result);
	publishValueMqtt(result, DHT_HUMIDITY_TOPIC);
	return result;
}

float retrieveTemperature() {
	float value = SIMULATED ? random(15, 30) : sensor.readTemperature();
	if (isnan(value)) {
		Serial.println("Problem reading sensor value.");
		return -999;
	}

	Serial.print("Temperature sensor value:");
	Serial.println(value);
	publishValueMqtt(value, DHT_TEMPERATURE_TOPIC);
	return value;
}

void publishValueMqtt(double value, const char* topic) {
	char strValue[10];
	dtostrf(value, 3, 2, strValue);
	mqttClient.publish(topic, strValue);
}

void controlHumidity(int humidityValue) {
	Serial.printf("Measured humidity value: %d \n", humidityValue);
	if (humidityValue > DESIRED_HUMIDITY) {
		if (humidityValue > HIGH_TRESHOLD_VALUE) {
			updatePinState(FAN_SPEED_PIN, ON);
		} else {
			updatePinState(FAN_SPEED_PIN, OFF);
		}

		updatePinState(FAN_POWER_PIN, ON);
	} else {
		updatePinState(FAN_POWER_PIN, OFF);
		updatePinState(FAN_SPEED_PIN, OFF);
	}
	Serial.printf("Fan power pin state = %d\n", digitalRead(FAN_POWER_PIN.getPin()));
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

void mqttCallback(char* topic, byte* payload, unsigned int length) {
	Serial.println("Received update via MQTT ! ! !");
	char cPayload[10];
	for (unsigned int i = 0; i <= length; i++) {
		cPayload[i] = (char) payload[i];
	}
	cPayload[length] = '\0';
	Serial.printf("Received message: topic=%s, payload=%s\n", topic, cPayload);
}

void loop() {
	sensorsUpdateTrigger.update();
	mqttClient.loop();
}
