
#ifndef __USER_CONFIG_H__
#define __USER_CONFIG_H__

#ifndef __MQTT_CONFIG_H__
#define __MQTT_CONFIG_H__

// security.h has definition of SSID and Password for Wi-fi
#include "D:\HomeAutomation\ESP8266\security.h"

//#define MQTT_SSL_ENABLE

#define MQTT_HOST     "192.168.254.40"
#define MQTT_PORT     1883
#define MQTT_BUF_SIZE   1024
#define MQTT_KEEPALIVE    120  /*second*/

// MQTT USER CONFIG
#define MQTT_CLIENT_ID    "EntranceOutsideLight"
//#define MQTT_USER     ""
//#define MQTT_PASS     ""
#define MQTT_CLEAN_SESSION 1
#define MQTT_KEEPALIVE 120

#define STA_SSID WIFI_SSID
#define STA_PASS WIFI_PASSWORD
#define STA_TYPE AUTH_WPA2_PSK

#define MQTT_RECONNECT_TIMEOUT  5 /*second*/

#define DEFAULT_SECURITY  0
#define QUEUE_BUFFER_SIZE       2048

#define PROTOCOL_NAMEv31  /*MQTT version 3.1 compatible with Mosquitto v0.15*/
//PROTOCOL_NAMEv311     /*MQTT version 3.11 compatible with https://eclipse.org/paho/clients/testing/*/
#define DEBUG_ON

#if defined(DEBUG_ON)
#define INFO( format, ... ) os_printf( format, ## __VA_ARGS__ )
#else
#define INFO( format, ... )
#endif

#endif // __MQTT_CONFIG_H__

#define PLATFORM_DEBUG		true

// Static definitions from MQTT user_init.c
// GPIO settings - all GPIOs set as GPIO
// DS18B20
#define PIN_GPIO2 2
#define PIN_GPIO2_MUX PERIPHS_IO_MUX_GPIO2_U
#define PIN_GPIO2_FUNC FUNC_GPIO2

// Output pin for the relay
#define PIN_GPIO4 4
#define PIN_GPIO4_MUX PERIPHS_IO_MUX_MTDI_U
#define PIN_GPIO4_FUNC FUNC_GPIO4
#define PIN_GPIO4_TOPIC "home/floor1/entrance/outside/light/1"
#define PIN_GPIO4_TOPIC_CB "home/floor1/entrance/outside/light/1cb"

#define PIN_GPIO5 5
#define PIN_GPIO5_MUX PERIPHS_IO_MUX_GPIO5_U
#define PIN_GPIO5_FUNC FUNC_GPIO5

#define PIN_GPIO12 12
#define PIN_GPIO12_MUX PERIPHS_IO_MUX_MTDI_U
#define PIN_GPIO12_FUNC FUNC_GPIO12

#define PIN_GPIO13 13
#define PIN_GPIO13_MUX PERIPHS_IO_MUX_MTCK_U
#define PIN_GPIO13_FUNC FUNC_GPIO13

#define PIN_GPIO14 14
#define PIN_GPIO14_MUX PERIPHS_IO_MUX_MTDI_U
#define PIN_GPIO14_FUNC FUNC_GPIO14

#define ENTRANCE_TEMPERATURE "home/floor1/entrance/temperature/1"


char currGPIO2State;
char currGPIO4State;
char currGPIO5State;
char currGPIO12State;
char currGPIO13State;
char currGPIO14State;
char sTemperature[10];
char sDate[20];

#define sleepms(x) os_delay_us(x*1000);

#define ON "ON"
#define OFF "OFF"


#endif
