// Only modify this file to include
// - function definitions (prototypes)
// - include files
// - extern variable definitions
// In the appropriate section

#ifndef _Aquarium_H_
#define _Aquarium_H_
#include "Arduino.h"
//add your includes for the project Aquarium here
#include <PubSubClient.h>
#include <DallasTemperature.h>
#include <Timer.h>
#include <UIPEthernet.h>
#include "userData.h"
#include "util.h"
//end of add your includes here

//add your function definitions for the project Aquarium here
byte server[] = MQTT_SERVER_IP_ADDRESS;

uint8_t mac[6] = ETH_MAC_ADDRESS;
IPAddress myIP(ETH_IP_ADDRESS);  //IP address of Arduino

OneWire oneWire(ONE_WIRE_PIN);
DallasTemperature sensors(&oneWire);

EthernetClient ethClient;
void mqttCallbackFunction(char* topic, byte* payload, unsigned int length);
PubSubClient mqttClient(server, 1883, mqttCallbackFunction, ethClient);

float desiredTemperature;
Mode mode;

//Do not add code below this line
#endif /* _Aquarium_H_ */
