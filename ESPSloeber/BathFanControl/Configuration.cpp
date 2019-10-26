/*
 * Configuration.cpp
 *
 *  Created on: Oct 25, 2019
 *      Author: theater
 */

#include "Configuration.h"

Configuration::Configuration() {
}

Configuration::~Configuration() {
	// TODO Auto-generated destructor stub
}

void Configuration::print() {
	Serial.printf("Current mode set to %d \n", mode);
	//TODO add here better serial printing of all values
}
