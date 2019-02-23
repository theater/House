/*
 * Pin.cpp
 *
 *  Created on: Feb 23, 2019
 *      Author: theater
 */

#include "Pin.h"

Pin::Pin(int pin, const char* topic) {
	this->pin = pin;
	this->topic = topic;
}

Pin::~Pin() {
	// TODO Auto-generated destructor stub
}
