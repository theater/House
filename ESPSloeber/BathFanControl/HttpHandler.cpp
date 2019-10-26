/*
 * HttpHandler.cpp
 *
 *  Created on: Oct 24, 2019
 *      Author: theater
 */

#include "HttpHandler.h"

HttpHandler::HttpHandler(Configuration *config) {
	this->config = config;
	this->httpServer = new ESP8266WebServer(80);
}

HttpHandler::~HttpHandler() {
	httpServer->~ESP8266WebServer();
}

void HttpHandler::handleRootRequest() {
	Serial.println("Received request to root folder...");
	String replacedValues = replaceHtmlValues(CONFIG_HTML);
	httpServer->send(200, "text/html", replacedValues);
}

void HttpHandler::handleSubmitRequest() {
	Serial.println("Received request to submit folder...");

	String desiredHumidity = httpServer->arg("desiredHumidity");
	Serial.printf("Desired humidity received: %s", desiredHumidity.c_str());

	String response = "<html>Submit triggered. Received=";
	response.concat(desiredHumidity.c_str());
	response.concat("</html");
	httpServer->send(200, "text/html", response);
}

void HttpHandler::init() {
	Serial.println("Creating navigation bindings...");
	on("/", std::bind(&HttpHandler::handleRootRequest, this));
	on("/submit", std::bind(&HttpHandler::handleSubmitRequest, this));
	begin();
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

String HttpHandler::replaceHtmlValues(String html) {
	html.replace("ssidValue", config->ssid);

	html.replace("mqttServerAddressValue", config->mqttServerAddress.toString());
	html.replace("mqttPortValue", String(config->mqttPort));
	html.replace("mqttClientNameValue", config->mqttClientName);

	html.replace("modeMqttTopicValue", config->modeMqttTopic);
	html.replace("desiredHumidityMqttTopicValue", config->desiredHumidityMqttTopic);
	html.replace("temperatureMqttTopicValue", config->temperatureMqttTopic);
	html.replace("humidityMqttTopicValue", config->humidityMqttTopic);
	html.replace("fanSpeedMqttTopicValue", config->fanSpeedMqttTopic);

	html.replace("desiredHumidityValue", String(config->desiredHumidity));
	html.replace("lowSpeedTresholdValue", String(config->lowSpeedTreshold));
	html.replace("highSpeedTresholdValue", String(config->highSpeedTreshold));
	return html;
}
