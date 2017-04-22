/*
 * userDS18B20.h
 *
 *  Created on: 10.04.2017 ã.
 *      Author: theater
 */

#ifndef INCLUDE_USERDS18B20_H_
#define INCLUDE_USERDS18B20_H_

#define DELAY 60000 // milliseconds
#define NTPDELAY 30000 // milliseconds
LOCAL os_timer_t ds18b20_timer;
int ds18b20();
LOCAL void ds18b20_cb(void *arg);


#endif /* INCLUDE_USERDS18B20_H_ */
