/*
 * userDS18B20.c
 *
 *  Created on: 10.04.2017 ã.
 *      Author: theater
 */

#include <c_types.h>
#include <os_type.h>
#include <osapi.h>
#include <user_interface.h>

#include "../include/driver/ds18b20.h"
#include "../include/user_config.h"
#include "../include/userMqtt.h"
#include "../mqtt/include/mqtt.h"

//===========================================================
//		Kosio implementations
//===========================================================
int ICACHE_FLASH_ATTR ds18b20() {
	int r, i;
	uint8_t addr[8], data[12];
	ds_init();

	r = ds_search(addr);
	if (r) {
		INFO("Found Device @ %02x %02x %02x %02x %02x %02x %02x %02x\r\n", addr[0], addr[1], addr[2], addr[3], addr[4], addr[5], addr[6],
				addr[7]);
		if (crc8(addr, 7) != addr[7])
			INFO("CRC mismatch, crc=%xd, addr[7]=%xd\r\n", crc8(addr, 7), addr[7]);

		switch (addr[0]) {
			case 0x10:
				INFO("Device is DS18S20 family.\r\n");
				break;

			case 0x28:
				INFO("Device is DS18B20 family.\r\n");
				break;

			default:
				INFO("Device is unknown family.\r\n");
				return 1;
		}
	} else {
		INFO("No DS18B20 detected, sorry.\r\n");
		return 1;
	}
	// perform the conversion
	reset();
	select(addr);

	write(DS1820_CONVERT_T, 1); // perform temperature conversion

	sleepms(1000); // sleep 1s

	INFO("Scratchpad: ");
	reset();
	select(addr);
	write(DS1820_READ_SCRATCHPAD, 0); // read scratchpad

	for (i = 0; i < 9; i++) {
		data[i] = read();
		INFO("%2x ", data[i]);
	}
	INFO("\r\n");

	int highByte, lowByte, tReading, signBit, Tc_100, whole, fract;
	lowByte = data[0];
	highByte = data[1];
	tReading = (highByte << 8) + lowByte;
	signBit = tReading & 0x8000;  // test most sig bit
	if (signBit) // negative
		tReading = (tReading ^ 0xffff) + 1; // 2's comp

	whole = tReading >> 4;  // separate off the whole and fractional portions
	fract = (tReading & 0xf) * 100 / 16;

	INFO("Temperature: %c%d.%d Celsius\r\n", signBit ? '-' : '+', whole, fract < 10 ? 0 : fract);
	if (signBit) {
		os_sprintf(sTemperature, "%c%d.%d", '-', whole, fract < 10 ? 0 : fract);
		MQTT_Publish(&mqttClient, ENTRANCE_TEMPERATURE, sTemperature, 6, 0, 0);
	} else {
		os_sprintf(sTemperature, "%d.%d", whole, fract < 10 ? 0 : fract);
		MQTT_Publish(&mqttClient, ENTRANCE_TEMPERATURE, sTemperature, 5, 0, 0);
	}
	return r;
}
