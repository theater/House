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

// PINS:
#define INPUT_PIN_IR_TRIGGER 3
#define HEAT_PIN 6
#define LIGHTS_PIN 5
#define DHT_PIN 8

// PROXIMITY SENSOR SETTINGS
#define TIMER_OFFSET 20000
#define DEBOUNCE_TIMEOUT 200
#define REATTACH_INTERRUPT_OFFSET 1000
// DHT SENSOR TIMER SETTINGS
#define DHT_TIMER_EVERY 30000

bool debug = true;

struct DhtSensorData {
	float humidity;
	float temperature;
};

void setRelay(byte pin, boolean state);
void decisionMaker();
void readDhtInfo();
void offTimerExpired();
void userInputDetected();
void reattachInterrupt();
boolean lightIsOn();
boolean heatingIsOn();

//Do not add code below this line
#endif /* _BathRoomMirrorHeatingCtrl_H_ */
