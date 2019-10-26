// HTML web page to handle 3 input fields (input1, input2, input3)
const char CONFIG_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
	<head>
		<h1>ESP8266 Configuration page</h1>
		<title>ESP8266 Configuration page</title>
  		<meta name="viewport" content="width=device-width, initial-scale=1">
 	</head>
  
	<body>
		<form action="/submit" method=post>
    		Simulation yes/no: <input type="checkbox" name="isSimulated"><br>
   			<fieldset>
    			<legend>Wi-fi Information:</legend>
    			WIFI SSID: <input type="text" name="ssid" value="ssidValue"><br>
    			WIFI Password: <input type="text" name="ssidPassword" value="********"><br>
   			</fieldset>
   			<fieldset>
    		<legend>MQTT Connection Information:</legend>
    			MQTT Server Address: <input type="text" name="mqttServerAddress" value="mqttServerAddressValue"><br>
    			MQTT Server Port: <input type="text" name="mqttPort" value=1883 value="mqttPortValue"><br>
    			MQTT Client Name: <input type="text" name="mqttClientName" value="mqttClientNameValue"><br>
   			</fieldset>
   			<fieldset>
    			<legend>MQTT Topics:</legend>
    			Device Mode Topic: <input type="text" name="modeMqttTopic" value="modeMqttTopicValue"><br>
    			Desired Humidity Topic: <input type="text" name="desiredHumidityMqttTopic" value="desiredHumidityMqttTopicValue"><br>
    			Current Temperature Topic: <input type="text" name="temperatureMqttTopic" value="temperatureMqttTopicValue"><br>
    			Current Humidity Topic: <input type="text" name="humidityMqttTopic" value="humidityMqttTopicValue"><br>
    			Current Fan Speed Topic: <input type="text" name="fanSpeedMqttTopic" value="fanSpeedMqttTopicValue"><br>
   			</fieldset>
   			<fieldset>
    			<legend>Device Logic Settings:</legend>
    			Desired humidity: <input type="text" name="desiredHumidity" value="desiredHumidityValue"><br>
    			Low Speed Humidity Treshold: <input type="text" name="lowSpeedTreshold" value="lowSpeedTresholdValue"><br>
    			High Speed Humidity Treshold: <input type="text" name="highSpeedTreshold" value="highSpeedTresholdValue"><br>
   			</fieldset>
    		<input type="submit" value="Submit">
  		</form>
	</body>
</html>)rawliteral";