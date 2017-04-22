#include <c_types.h>
#include <os_type.h>
#include <osapi.h>
#include <user_interface.h>

#include "../include/driver/uart.h"
#include "../include/user_config.h"
#include "../include/userDS18B20.h"
#include "../include/userMqtt.h"
#include "../modules/include/wifi.h"
#include "../mqtt/include/mqtt.h"

void ICACHE_FLASH_ATTR wifiConnectCb(uint8_t status) {
	if (status == STATION_GOT_IP) {
		MQTT_Connect(&mqttClient);
	} else {
		MQTT_Disconnect(&mqttClient);
	}
}

// DS18B20
LOCAL void ICACHE_FLASH_ATTR ds18b20_cb(void *arg) {
	INFO("DS18B20 CB executed\n");
	ds18b20();
}

void printConfiguration() {
	//	put here some debug information
}

uint32 ICACHE_FLASH_ATTR user_rf_cal_sector_set(void) {
	enum flash_size_map size_map = system_get_flash_size_map();
	uint32 rf_cal_sec = 0;

	switch (size_map) {
		case FLASH_SIZE_4M_MAP_256_256:
			rf_cal_sec = 128 - 8;
			break;

		case FLASH_SIZE_8M_MAP_512_512:
			rf_cal_sec = 256 - 5;
			break;

		case FLASH_SIZE_16M_MAP_512_512:
		case FLASH_SIZE_16M_MAP_1024_1024:
			rf_cal_sec = 512 - 5;
			break;

		case FLASH_SIZE_32M_MAP_512_512:
		case FLASH_SIZE_32M_MAP_1024_1024:
			rf_cal_sec = 1024 - 5;
			break;

		default:
			rf_cal_sec = 0;
			break;
	}

	return rf_cal_sec;
}

void ICACHE_FLASH_ATTR user_rf_pre_init(void) {
}

void ICACHE_FLASH_ATTR user_init(void) {
	uart_init(BIT_RATE_115200, BIT_RATE_115200);
	os_delay_us(1000000);

	#if defined(DEBUG_ON)
		printConfiguration();
	#endif

//	MQTT_InitConnection(&mqttClient, sysCfg.mqtt_host, sysCfg.mqtt_port, sysCfg.security);
	MQTT_InitConnection(&mqttClient, MQTT_HOST, MQTT_PORT, DEFAULT_SECURITY);

//	MQTT_InitClient(&mqttClient, sysCfg.device_id, sysCfg.mqtt_user, sysCfg.mqtt_pass, sysCfg.mqtt_keepalive, 1);
	MQTT_InitClient(&mqttClient, "EntranceLamp", "user", "pass", 120, 1);

//	MQTT_InitLWT(&mqttClient, "/lwt", "offline", 0, 0);
	MQTT_OnConnected(&mqttClient, mqttConnectedCb);
	MQTT_OnDisconnected(&mqttClient, mqttDisconnectedCb);
	MQTT_OnPublished(&mqttClient, mqttPublishedCb);
	MQTT_OnData(&mqttClient, mqttDataCb);

	//DS18B20 timers
	os_timer_disarm(&ds18b20_timer);
	os_timer_setfn(&ds18b20_timer, (os_timer_func_t *) ds18b20_cb, (void *) 0);
	os_timer_arm(&ds18b20_timer, DELAY, 1);

//	WIFI_Connect(sysCfg.sta_ssid, sysCfg.sta_pwd, wifiConnectCb);
	WIFI_Connect(STA_SSID, STA_PASS, wifiConnectCb);

	INFO("\r\nSystem started ...\r\n");
}
