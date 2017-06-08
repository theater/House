// Only modify this file to include
// - function definitions (prototypes)
// - include files
// - extern variable definitions
// In the appropriate section

#ifndef _BathRoomMirrorHeatingCtrl_H_
#define _BathRoomMirrorHeatingCtrl_H_
#include "Arduino.h"

#define ON 0x1
#define OFF 0x0

// TRESHOLDS
#define HUMIDITY_TRESHOLD 65
#define ANALOG_TRESHOLD 350

// PINS:
#define INPUT_PIN_IR_TRIGGER A2			// IR pin is Analog 2
#define HEAT_PIN 7
#define LIGHTS_PIN 6
#define DHT_PIN 5

// PROXIMITY SENSOR SETTINGS
#define OFF_TIME_TIMER_OFFSET 900000 	// 15 min hard off time
#define RESET_ANALOG_SENSOR_READING 500 // something like debounce time

// DHT SENSOR TIMER SETTINGS
#define DHT_TIMER_EVERY 30000			// DHT read on every 30 sec

struct DhtSensorData {
	float humidity;
	float temperature;
};

void setRelay(byte pin, byte state);
void decisionMaker();
void readDhtInfo();
void offTimerExpired();
void userInputDetected();
void reattachInterrupt();
boolean lightIsOn();
boolean heatingIsOn();
void analogReadInputPin();
void resetReadingAndUpdateDecision();

//Do not add code below this line
#endif /* _BathRoomMirrorHeatingCtrl_H_ */
