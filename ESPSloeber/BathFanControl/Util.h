/*
 * Util.h
 *
 *  Created on: Feb 23, 2019
 *      Author: theater
 */

#ifndef UTIL_H_
#define UTIL_H_


bool wifiConnect(const char* ssid, const char* password);
bool handleHtmlCheckBox(String value);
String booleanToCheckBoxResponse(bool value);

#endif /* UTIL_H_ */
