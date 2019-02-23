/*
 * Pin.h
 *
 *  Created on: Feb 23, 2019
 *      Author: theater
 */

#ifndef PIN_H_
#define PIN_H_

class Pin {
	private:
		const char* topic;
		int pin;
	public:
		Pin(int pin, const char* topic);
		virtual ~Pin();

		int getPin() const {
			return pin;
		}

		const char* getTopic() const {
			return topic;
		}
};

#endif /* PIN_H_ */
