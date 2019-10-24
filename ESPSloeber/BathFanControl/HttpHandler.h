/*
 * HttpHandler.h
 *
 *  Created on: Oct 24, 2019
 *      Author: theater
 *      Wrapper of ESP8266WebServer with additional functionality
 */

#ifndef HTTPHANDLER_H_
#define HTTPHANDLER_H_
#include <ESP8266WebServer.h>
#include <functional>

class HttpHandler {
	private:
		typedef std::function<void(void)> THandlerFunction;
		ESP8266WebServer *httpServer;
	public:
		HttpHandler();
		virtual ~HttpHandler();
		void handleRootRequest();
  	    void on(const char* uri, THandlerFunction handler);
  	    void begin();
  	    void handleClient();
};

#endif /* HTTPHANDLER_H_ */
