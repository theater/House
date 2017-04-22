/*
 * userMqtt.h
 *
 *  Created on: 10.04.2017 ã.
 *      Author: theater
 */

#ifndef INCLUDE_USERMQTT_H_
#define INCLUDE_USERMQTT_H_

#include <c_types.h>

#include "../mqtt/include/mqtt.h"

MQTT_Client mqttClient;

void mqttConnectedCb(uint32_t *args);
void mqttDisconnectedCb(uint32_t *args);
void mqttPublishedCb(uint32_t *args);
void mqttDataCb(uint32_t *args, const char* topic, uint32_t topic_len,
		const char *data, uint32_t data_len);

#endif /* INCLUDE_USERMQTT_H_ */
