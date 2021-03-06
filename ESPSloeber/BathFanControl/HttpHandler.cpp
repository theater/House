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

void HttpHandler::handleSaveRequest() {
	Serial.println("Received request to submit folder...");

	transferArgumentsToConfig();
	config->saveEprom();

	String replacedValues = replaceHtmlValues(CONFIG_HTML, "Successfully saved config to EPROM.");
	httpServer->send(200, "text/html", replacedValues);
}

void HttpHandler::handleLoadRequest() {
	Serial.println("Received request to load EPROM...");
	config->loadEprom();

	String replacedValues = replaceHtmlValues(CONFIG_HTML, "Successfully loaded config from EPROM.");
	httpServer->send(200, "text/html", replacedValues);
}

void HttpHandler::handleManualModeRequest() {
	Serial.println("Received manual mode request..");
	if (httpServer->arg("HeaterON").equals("HeaterON")) {

	} else if (httpServer->arg("HeaterOFF").equals("HeaterOFF")) {

	} else if (httpServer->arg("FanSpeedHIGH").equals("FanSpeedHIGH")) {

	} else if (httpServer->arg("FanSpeedLOW").equals("FanSpeedLOW")) {

	}
	httpServer->send(200, "text/html", MANUAL_HTML);
}

void HttpHandler::init() {
	Serial.println("Creating navigation bindings...");
	on("/", std::bind(&HttpHandler::handleRootRequest, this));
	on("/submit", std::bind(&HttpHandler::handleSaveRequest, this));
	on("/load", std::bind(&HttpHandler::handleLoadRequest, this));
	on("/manual", std::bind(&HttpHandler::handleManualModeRequest, this));
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
	return replaceHtmlValues(html, "");
}
String HttpHandler::replaceHtmlValues(String html, String status) {
	html.replace("ssidValue", config->ssid);

	html.replace("mqttServerAddressValue", config->mqttServerAddress.toString());
	html.replace("mqttPortValue", String(config->mqttPort));
	html.replace("mqttClientNameValue", config->mqttClientName);

	html.replace("modeMqttTopicValue", config->modeMqttTopic);
	html.replace("desiredHumidityMqttTopicValue", config->desiredHumidityMqttTopic);
	html.replace("temperatureMqttTopicValue", config->temperatureMqttTopic);
	html.replace("humidityMqttTopicValue", config->humidityMqttTopic);
	html.replace("fanSpeedMqttTopicValue", config->fanSpeedMqttTopic);
	html.replace("mirrorHeatingMqttTopicValue", config->mirrorHeatingMqttTopic);

	html.replace("modeValue", String(config->mode));
	html.replace("desiredHumidityValue", String(config->desiredHumidity));
	html.replace("highSpeedThresholdValue", String(config->highSpeedThresholdDelta));
	html.replace("humidityToleranceValue", String(config->humidityTolerance));
	html.replace("temperatureCorrectionValue", String(config->temperatureCorrection));
	html.replace("humidityCorrectionValue", String(config->humidityCorrection));

	html.replace("StatusValue", status);

	return html;
}

void HttpHandler::transferArgumentsToConfig() {
	config->updateValue(&config->isSimulated, httpServer->arg("isSimulated"), "isSimulated");

	////////////////////////////////////////////////////////////////
	config->updateValue(&config->ssid, httpServer->arg("ssid"), "ssid");
	config->updateValue(&config->ssidPassword, httpServer->arg("ssidPassword"), "ssidPassword");

	String mqttServerAddress = httpServer->arg("mqttServerAddress");
	if (!mqttServerAddress.equals("")) {
		config->mqttServerAddress.fromString(mqttServerAddress);
		Serial.printf("Updating IP address to value: %s\n", config->mqttServerAddress.toString().c_str());
	}
	config->updateValue(&config->mqttPort, httpServer->arg("mqttPort"), "mqttPort");
	config->updateValue(&config->mqttClientName, httpServer->arg("mqttClientName"), "mqttClientName");

	////////////////////////////////////////////////////////////////
	config->updateValue(&config->modeMqttTopic, httpServer->arg("modeMqttTopic"), "modeMqttTopic");
	config->updateValue(&config->desiredHumidityMqttTopic, httpServer->arg("desiredHumidityMqttTopic"), "desiredHumidityMqttTopic");
	config->updateValue(&config->temperatureMqttTopic, httpServer->arg("temperatureMqttTopic"), "temperatureMqttTopic");
	config->updateValue(&config->humidityMqttTopic, httpServer->arg("humidityMqttTopic"), "humidityMqttTopic");
	config->updateValue(&config->fanSpeedMqttTopic, httpServer->arg("fanSpeedMqttTopic"), "fanSpeedMqttTopic");
	config->updateValue(&config->mirrorHeatingMqttTopic, httpServer->arg("mirrorHeatingMqttTopic"), "mirrorHeatingMqttTopic");

	////////////////////////////////////////////////////////////////
	config->updateValue(&config->mode, httpServer->arg("mode"), "mode");
	config->updateValue(&config->desiredHumidity, httpServer->arg("desiredHumidity"), "desiredHumidity");
	config->updateValue(&config->highSpeedThresholdDelta, httpServer->arg("highSpeedThreshold"), "highSpeedThreshold");
	config->updateValue(&config->temperatureCorrection, httpServer->arg("temperatureCorrection"), "temperatureCorrection");
	config->updateValue(&config->humidityCorrection, httpServer->arg("humidityCorrection"), "humidityCorrection");
	config->updateValue(&config->humidityTolerance, httpServer->arg("humidityTolerance"), "humidityTolerance");
}
