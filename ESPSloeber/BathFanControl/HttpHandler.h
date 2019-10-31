/*
 * HttpHandler.h
 *
 *  Created on: Oct 24, 2019
 *      Author: theater
 *      Wrapper of ESP8266WebServer with additional functionality specific to my use-case
 */

#ifndef HTTPHANDLER_H_
#define HTTPHANDLER_H_
#include <ESP8266WebServer.h>
#include <functional>
#include "resources/MainConfigPage.h"
#include "Configuration.h"
#include "Util.h"

class HttpHandler {
	private:
		typedef std::function<void(void)> THandlerFunction;
		ESP8266WebServer *httpServer;
		Configuration *config;
		String replaceHtmlValues(String html);
		void transferArgumentsToConfig();

	public:
		HttpHandler(Configuration * config);
		virtual ~HttpHandler();
		void handleRootRequest();
		void handleSubmitRequest();
		void handleLoadRequest();
  	    void on(const char* uri, THandlerFunction handler);
  	    void begin();
  	    void init();
  	    void handleClient();
};

#endif /* HTTPHANDLER_H_ */
