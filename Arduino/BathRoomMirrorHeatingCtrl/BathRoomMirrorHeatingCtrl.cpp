#include "BathRoomMirrorHeatingCtrl.h"

#include <HardwareSerial.h>
#include <stdio.h>

#include "C:/eclipseArduino/sloeber/arduinoPlugin/packages/arduino/hardware/avr/1.6.18/variants/standard/pins_arduino.h"
#include "D:/HomeAutomation/ArduinoLibs/DHT/DHT.h"
#include "D:/HomeAutomation/ArduinoLibs/Timer/Timer.h"

Timer reattachInterruptTimer;
Timer offTimer;
Timer sensorReadTimer;
DHT dhtSensor(DHT_PIN, DHT22);
volatile DhtSensorData sensorData;
volatile boolean isHeatingAndLightDesired;

bool debug = true;

//The setup function is called once at startup of the sketch
void setup() {
	Serial.begin(115200);
	Serial.println("Setup starts here...");

	isHeatingAndLightDesired = false;
	sensorData.temperature = 25;
	sensorData.humidity = 55;

	pinMode(INPUT_PIN_IR_TRIGGER, INPUT);
	attachInterrupt(digitalPinToInterrupt(INPUT_PIN_IR_TRIGGER), userInputDetected, RISING);

	pinMode(LIGHTS_PIN, OUTPUT);
	setRelay(LIGHTS_PIN, OFF);

	pinMode(HEAT_PIN, OUTPUT);
	setRelay(HEAT_PIN, OFF);

	dhtSensor.begin();
	readDhtInfo();
	sensorReadTimer.every(DHT_TIMER_EVERY, readDhtInfo);
	Serial.println("Setup ends here...");
}

// The loop function is called in an endless loop
void loop() {
	offTimer.update();
	sensorReadTimer.update();
	reattachInterruptTimer.update();
	decisionMaker();
}

void userInputDetected() {
	detachInterrupt(digitalPinToInterrupt(INPUT_PIN_IR_TRIGGER));
	if(debug) {
		Serial.println("IR triggered");
		Serial.println(isHeatingAndLightDesired);
	}
	delay(15);
	if (digitalRead(INPUT_PIN_IR_TRIGGER)) {
		delay(DEBOUNCE_TIMEOUT);
		if (digitalRead(INPUT_PIN_IR_TRIGGER)) {
			isHeatingAndLightDesired = !isHeatingAndLightDesired;
			if (isHeatingAndLightDesired) {
				offTimer.after(OFF_TIME_TIMER_OFFSET, offTimerExpired);
			} else {
				offTimer.stop(0);
			}
		}
	}
	delay(REATTACH_INTERRUPT_OFFSET);
	attachInterrupt(digitalPinToInterrupt(INPUT_PIN_IR_TRIGGER), userInputDetected, RISING);
	Serial.println(isHeatingAndLightDesired);
}

void offTimerExpired() {
	isHeatingAndLightDesired = false;;
}

void reattachInterrupt() {
	if(debug) {
		Serial.println("ReattachInterrupt triggered");
	}
	attachInterrupt(digitalPinToInterrupt(INPUT_PIN_IR_TRIGGER), userInputDetected, RISING);
	reattachInterruptTimer.stop(millis());
}

void readDhtInfo() {
	float humidity = dhtSensor.readHumidity();
	float temperature = dhtSensor.readTemperature();

	// simple error check. Do not update data if sensor reading is wrong.
	if(humidity > 0 && temperature > -30) {
		sensorData.humidity = humidity;
		sensorData.temperature = temperature;
	} else if(debug) {
		Serial.println("DHT wrong data received...");
		Serial.print("Humidity:"); Serial.println(humidity);
		Serial.print("Temperature:"); Serial.println(temperature);
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
		if(sensorData.humidity > HUMIDITY_TRESHOLD && !heatingIsOn()) {
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
