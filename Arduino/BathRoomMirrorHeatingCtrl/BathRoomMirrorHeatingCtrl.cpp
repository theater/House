#include "BathRoomMirrorHeatingCtrl.h"

#include <HardwareSerial.h>
#include <stdio.h>

#include "C:/eclipseArduino/sloeber/arduinoPlugin/packages/arduino/hardware/avr/1.6.18/variants/standard/pins_arduino.h"
#include "D:/HomeAutomation/ArduinoLibs/DHT/DHT.h"
#include "D:/HomeAutomation/ArduinoLibs/Timer/Timer.h"

Timer resetAnalogPinTimer;
Timer offTimer;
Timer humiditySensorReadTimer;
DHT dhtSensor(DHT_PIN, DHT22);
volatile DhtSensorData sensorData;
volatile boolean isHeatingAndLightDesired;
volatile int analogSensorReading;

bool debug = true;
bool ignoreHumiditySensor = true;

//The setup function is called once at startup of the sketch
void setup() {
	Serial.begin(115200);
	Serial.println("Setup starts here...");

	isHeatingAndLightDesired = false;
	sensorData.temperature = 25;
	sensorData.humidity = 55;

	pinMode(INPUT_PIN_IR_TRIGGER, INPUT);
	analogReadInputPin();
	resetAnalogPinTimer.every(RESET_ANALOG_SENSOR_READING, resetReadingAndUpdateDecision);

	pinMode(LIGHTS_PIN, OUTPUT);
	setRelay(LIGHTS_PIN, OFF);

	pinMode(HEAT_PIN, OUTPUT);
	setRelay(HEAT_PIN, OFF);

	dhtSensor.begin();
	readDhtInfo();
	humiditySensorReadTimer.every(DHT_TIMER_EVERY, readDhtInfo);
	Serial.println("Setup ends here...");
}

// The loop function is called in an endless loop
void loop() {
	offTimer.update();
	humiditySensorReadTimer.update();
	resetAnalogPinTimer.update();
	decisionMaker();
	analogReadInputPin();
}

void resetReadingAndUpdateDecision() {
	if(analogSensorReading > ANALOG_TRESHOLD) {
		isHeatingAndLightDesired = !isHeatingAndLightDesired;
	}
	Serial.print("AnalogSensor last value:");
	Serial.println(analogSensorReading);
	analogSensorReading = 0;
	Serial.print("AnalogSensor new value:");
	Serial.println(analogSensorReading);
}

void offTimerExpired() {
	isHeatingAndLightDesired = false;;
}

void reattachInterrupt() {
	if(debug) {
		Serial.println("ReattachInterrupt triggered");
	}
	attachInterrupt(digitalPinToInterrupt(INPUT_PIN_IR_TRIGGER), userInputDetected, RISING);
	resetAnalogPinTimer.stop(millis());
}

void readDhtInfo() {
	float humidity = dhtSensor.readHumidity();
	float temperature = dhtSensor.readTemperature();

	// simple error check. Do not update data if sensor reading is wrong.
	if(humidity > 0 && temperature > -30) {
		sensorData.humidity = humidity;
		sensorData.temperature = temperature;
		ignoreHumiditySensor = false;
	} else  {
		ignoreHumiditySensor = true;
		if(debug) {
			Serial.println("DHT wrong data received...");
			Serial.print("Humidity:"); Serial.println(humidity);
			Serial.print("Temperature:"); Serial.println(temperature);
		}
	}
	if(debug) {
//		sensorData.humidity = random(100);
//		sensorData.temperature = random(30);
		Serial.print("Humidity:");Serial.println(sensorData.humidity);
		Serial.print("Temperature:");Serial.println(sensorData.temperature);
		return;
	}
}

void decisionMaker() {
	if(isHeatingAndLightDesired) {
		if(!lightIsOn()) {
			setRelay(LIGHTS_PIN, ON);
		}
		if((sensorData.humidity > HUMIDITY_TRESHOLD || ignoreHumiditySensor) && !heatingIsOn()) {
			setRelay(HEAT_PIN, ON);
		}
	} else {
		if(lightIsOn()) {
			setRelay(LIGHTS_PIN, OFF);
		}
		if(heatingIsOn()) {
			setRelay(HEAT_PIN, OFF);
		}
	}
}

void setRelay(byte pin, byte state) {
	digitalWrite(pin, state);
	if (debug) {
		Serial.print("Pin ");Serial.print(pin);Serial.print(" state changed to ");Serial.println(state);
	}
}

boolean lightIsOn() {
	return digitalRead(LIGHTS_PIN);
}

boolean heatingIsOn() {
	return digitalRead(HEAT_PIN);
}

void analogReadInputPin() {
	int sensorReading = analogRead(INPUT_PIN_IR_TRIGGER);
	if(sensorReading > 100 && sensorReading > analogSensorReading) {
		analogSensorReading = sensorReading;
	}
}
