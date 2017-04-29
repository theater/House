/*
 * userData.h
 *
 *  Created on: 24.04.2017 ã.
 *      Author: theater
 */

#ifndef USERDATA_H_
#define USERDATA_H_

#include <Arduino.h>
#include <IPAddress.h>


//Digital pins 10,11,12,13 are used for ethernet shield ! ! !

// Data wire is plugged into pin 4 on the Arduino
#define ONE_WIRE_PIN 9

// Aquarium mode mqtt
#define AQ_MODE_TOPIC "home/aquarium/1/mode"

// Aquarium set temperature mqtt
#define AQ_DESIRED_TEMPERATURE_TOPIC "home/aquarium/1/desired/temperature"

// Set temperature in manual mode (if no MQTT connection available)
#define AQ_DESIRED_TEMPERATURE_DEFAULT 25

// Temperature sensor of water
#define AQ_WATER_TEMP_SENSOR1_TOPIC "home/aquarium/1/temperature"

// Temperature sensor outside of aquarium
#define AQ_OUTSIDE_SENSOR1_TOPIC "home/floor1/storage/temperature/1"

//AQ_Lights1
#define AQ_LIGHTS1_PIN 7
#define AQ_LIGHTS_TOPIC "home/aquarium/1/light"
#define AQ_LIGHTS_CB_TOPIC "home/aquarium/1/lightcb"

//AQ_Heater1
#define AQ_HEATER1_PIN 6
#define AQ_HEATER1_TOPIC "home/aquarium/1/heater"
#define AQ_HEATER1_CB_TOPIC "home/aquarium/1/heatercb"

//AQ_Cooler1
#define AQ_COOLER1_PIN 5
#define AQ_COOLER1_TOPIC "home/aquarium/1/cooler"
#define AQ_COOLER1_CB_TOPIC "home/aquarium/1/coolercb"

// Reset button - pin 3 goes to GND
#define RESET 3

// report 1-wire sensors data each minute to MQTT broker
#define OW_REPORTING_INTERVAL 60000

// Aquarium manual function check interval will be on each 30 sec reset of ethernet on 5x30sec=150sec
#define AQ_MANUAL_INTERVAL 30000

// Reset counter will be changed each 1 mins, 3 times if not connected by then - reset the duino
#define RESET_INTERVAL 30000

// ID when connecting to MQTT server
#define MQTT_Client_ID "Aquarium1-30l"

#define MQTT_SERVER_IP_ADDRESS  {192,168,254,40}

#define ETH_MAC_ADDRESS {0x00,0x01,0x02,0x03,0x04,0x35}

#define ETH_IP_ADDRESS 192,168,254,35


#endif /* USERDATA_H_ */
