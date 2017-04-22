/*
 * userMqtt.c
 *
 *  Created on: 10.04.2017 ã.
 *      Author: theater
 */

#include "../include/userMqtt.h"

#include <c_types.h>
#include <gpio.h>
#include <string.h>
#include <osapi.h>
#include "../include/user_config.h"
#include "../mqtt/include/mqtt.h"
#include <mem.h>

void ICACHE_FLASH_ATTR mqttConnectedCb(uint32_t *args) {
	MQTT_Client* client = (MQTT_Client*) args;
	INFO("MQTT: Connected\r\n");
	MQTT_Subscribe(&mqttClient, PIN_GPIO4_TOPIC, 0);
//	MQTT_Publish(client, "/mqtt/topic/2", "hello2", 6, 2, 0);
}

void mqttDisconnectedCb(uint32_t *args) {
	MQTT_Client* client = (MQTT_Client*) args;
	INFO("MQTT: Disconnected\r\n");
}

void ICACHE_FLASH_ATTR mqttPublishedCb(uint32_t *args) {
	MQTT_Client* client = (MQTT_Client*) args;
	INFO("MQTT: Published\r\n");
}

void ICACHE_FLASH_ATTR mqttDataCb(uint32_t *args, const char* topic, uint32_t topic_len, const char *data, uint32_t data_len) {
	char topicBuf[64], dataBuf[64];
	MQTT_Client* client = (MQTT_Client*) args;

	os_memcpy(topicBuf, topic, topic_len);
	topicBuf[topic_len] = 0;

	os_memcpy(dataBuf, data, data_len);
	dataBuf[data_len] = 0;

	// GPIO4 handling here
	if (strcmp(topicBuf, PIN_GPIO4_TOPIC) == 0) {
		if (strcmp(dataBuf, "OFF") == 0) {
			GPIO_OUTPUT_SET(PIN_GPIO4, 0);
			INFO("GPIO4 set to OFF\r\n");
			MQTT_Publish(&mqttClient, PIN_GPIO4_TOPIC_CB, "OFF", 3, 0, 0);
		} else if (strcmp(dataBuf, "ON") == 0) {
			GPIO_OUTPUT_SET(PIN_GPIO4, 1);
			INFO("GPIO4 set to ON\r\n");
			MQTT_Publish(&mqttClient, PIN_GPIO4_TOPIC_CB, "ON", 2, 0, 0);
		}
	}
	// HERE ENDS THE BASIC MQTT related LOGIC BY KONSTANTIN

	//	INFO("GPIO2 State: %d",GPIO_INPUT_GET(PIN_GPIO));
	INFO("MQTT topic: %s, data: %s \r\n", topicBuf, dataBuf);
	os_free(topicBuf);
	os_free(dataBuf);
}
