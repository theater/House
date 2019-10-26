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

	transferArgumentsToConfig();
	config->saveEprom();

	httpServer->send(200, "text/plain", "Submit done");
}

void HttpHandler::init() {
	Serial.println("Creating navigation bindings...");
	on("/", std::bind(&HttpHandler::handleRootRequest, this));
	on("/submit", std::bind(&HttpHandler::handleSubmitRequest, this));
	on("/load", std::bind(&Configuration::loadEprom, config));
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

void HttpHandler::transferArgumentsToConfig() {
	config->updateValue(&config->isSimulated, httpServer->arg("isSimulated"), "isSimulated");

	////////////////////////////////////////////////////////////////
	config->updateValue(&config->ssid, httpServer->arg("ssid"), "ssid");
	config->updateValue(&config->ssidPassword, httpServer->arg("ssidPassword"), "ssidPassword");

	////////////////////////////////////////////////////////////////
	//TODO think about this later. Probably wont be too hard...
//	String mqttServerAddress = httpServer->arg("mqttServerAddress");
//	if (!mqttServerAddress.equals("")) {
//		config->updateValue(&config->mqttServerAddress, mqttServerAddress, "mqttServerAddress");
//	}
	config->updateValue(&config->mqttPort, httpServer->arg("mqttPort"), "mqttPort");
	config->updateValue(&config->mqttClientName, httpServer->arg("mqttClientName"), "mqttClientName");

	////////////////////////////////////////////////////////////////
	config->updateValue(&config->modeMqttTopic, httpServer->arg("modeMqttTopic"), "modeMqttTopic");
	config->updateValue(&config->desiredHumidityMqttTopic, httpServer->arg("desiredHumidityMqttTopic"), "desiredHumidityMqttTopic");
	config->updateValue(&config->temperatureMqttTopic, httpServer->arg("temperatureMqttTopic"), "temperatureMqttTopic");
	config->updateValue(&config->humidityMqttTopic, httpServer->arg("humidityMqttTopic"), "humidityMqttTopic");
	config->updateValue(&config->fanSpeedMqttTopic, httpServer->arg("fanSpeedMqttTopic"), "fanSpeedMqttTopic");

	////////////////////////////////////////////////////////////////
	config->updateValue(&config->desiredHumidity, httpServer->arg("desiredHumidity"), "desiredHumidity");
	config->updateValue(&config->lowSpeedTreshold, httpServer->arg("lowSpeedTreshold"), "lowSpeedTreshold");
	config->updateValue(&config->highSpeedTreshold, httpServer->arg("highSpeedTreshold"), "highSpeedTreshold");
}
