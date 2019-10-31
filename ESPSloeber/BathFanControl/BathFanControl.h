// Only modify this file to include
// - function definitions (prototypes)
// - include files
// - extern variable definitions
// In the appropriate section

#ifndef _BathFanControl_H_
#define _BathFanControl_H_
#include "Arduino.h"
//add your includes for the project BathFanControl here
#include "Pin.h"
#include "Messages.h"

//end of add your includes here


//add your function definitions for the project BathFanControl here
int retrieveHumidity();
float retrieveTemperature();
void controlHumidity(int humidityValue);
void updatePinState(Pin* pin, const byte state);
void mqttCallback(const char* topic, const byte* payload, const unsigned int length);
void publishValueMqtt(double value, const char* topic);
void timerUpdate();

enum MODE {OFF=0, ON=1, AUTO=2};

//Do not add code below this line
#endif /* _BathFanControl_H_ */
