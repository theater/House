// Do not remove the include below

#include "BathFanControl.h"

#include <PubSubClient.h>
#include <ESP8266WiFi.h>

#include "Util.h"

// Constants
const char* SSID = "KLP";
const char* PASSWORD = "charly78";
const IPAddress MQTT_SERVER = IPAddress({192,168,254,40});
const int MQTT_PORT = 1883;
const char* MQTT_CLIENT_NAME = "ESP_BATH_FLOOR1";
const int DESIRED_HUMIDITY = 60;
const int LOW_TRESHOLD_VALUE = 70;
const int HIGH_TRESHOLD_VALUE = 80;

const int DHT_PIN = 4;
const Pin FAN_POWER_PIN = Pin(0,"Topic1");
const Pin FAN_SPEED_PIN = Pin(2,"Topic2");

const byte OFF = 0;
const byte ON = 1;

// Variables
int humiditySensorValue = 60;
int temperatureSensorValue = 20;

// Objects
WiFiClient espClient;
PubSubClient mqttClient(espClient);

// Functions
void initializeGpioPinModes() {
	pinMode(FAN_POWER_PIN.getPin(), OUTPUT);
	pinMode(FAN_SPEED_PIN.getPin(), OUTPUT);
}

void loopUntilConnected() {
	while (!wifiConnect(SSID, PASSWORD)) {
	}

	mqttClient.setServer(MQTT_SERVER, MQTT_PORT);
	mqttClient.setCallback(mqttCallback);

	while (!mqttClient.connected()) {
		Serial.println("Connecting to MQTT...");

		if (mqttClient.connect(MQTT_CLIENT_NAME, "user", "password")) {
			Serial.println("connected");
		} else {
			Serial.print("failed with state ");
			Serial.println(mqttClient.state());
			delay(2000);
		}
	}

	mqttClient.publish(MQTT_CLIENT_NAME, "Hello from BATHROOM-FLOOR1");
//	  mqttClient.subscribe("esp/test");
}

void setup() {
	delay(1000);
	Serial.begin(115200);
	loopUntilConnected();
	initializeGpioPinModes();
}

void loop() {
	Serial.println("Start of loop...");
	humiditySensorValue = retrieveHumidity();
//	temperatureSensorValue = retrieveTemperature();
	humidityControl(humiditySensorValue);
	delay(5000);
}

int retrieveHumidity() {
	long value = random(40, 100);
	Serial.printf("Humidity sensor value: %d \n", value);
	return value;
}

int retrieveTemperature() {
	return random(15, 30);
}

void humidityControl(int measuredHumidity) {
	Serial.printf("Measured humidity value: %d \n", measuredHumidity);
	if (measuredHumidity > DESIRED_HUMIDITY) {
		if (measuredHumidity > HIGH_TRESHOLD_VALUE) {
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
	mqttClient.publish(pin.getTopic(), state ? "ON" : "OFF");
}

void mqttCallback(char* topic, byte* payload, unsigned int length) {

}
