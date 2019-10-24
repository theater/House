/*
 * HttpHandler.cpp
 *
 *  Created on: Oct 24, 2019
 *      Author: theater
 */

#include "HttpHandler.h"

HttpHandler::HttpHandler() {
	this->httpServer = new ESP8266WebServer(80);
}

HttpHandler::~HttpHandler() {
	httpServer->~ESP8266WebServer();
}

void HttpHandler::handleRootRequest() {
	Serial.println("Received request to root folder...");
	httpServer->send(200, "text/plain", "Hello world");
}

void HttpHandler::on(const char* uri, THandlerFunction handler) {
	httpServer->on(uri, handler);
}

void HttpHandler::begin() {
	Serial.println("Starting httpServer...");
	httpServer->begin();
}

void HttpHandler::handleClient() {
	httpServer->handleClient();
}
