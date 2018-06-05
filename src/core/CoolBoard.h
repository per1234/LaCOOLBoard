/**
 *  Copyright (c) 2018 La Cool Co SAS
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a
 *  copy of this software and associated documentation files (the "Software"),
 *  to deal in the Software without restriction, including without limitation
 *  the rights to use, copy, modify, merge, publish, distribute, sublicense,
 *  and/or sell copies of the Software, and to permit persons to whom the
 *  Software is furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included
 *  in all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 *  OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 *  THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 *  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 *  IN THE SOFTWARE.
 *
 */

#ifndef COOLBOARD_H
#define COOLBOARD_H

#include <Arduino.h>

#include "CoolBoardActuator.h"
#include "CoolBoardLed.h"
#include "CoolBoardSensors.h"
#include "CoolFileSystem.h"
#include "CoolTime.h"
#include "CoolWifi.h"
#include "ExternalSensors.h"
#include "Irene3000.h"
#include "Jetpack.h"
#include "CoolPubSubClient.h"
#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>

#define ENABLE_I2C_PIN 5
#define BOOTSTRAP_PIN 0

#define MQTT_RETRIES 5

class CoolBoard {

public:
  void begin();
  bool config();
  void update(const char *answer);
  void loop();
  int connect();
  bool isConnected();
  unsigned long getLogInterval();
  void printConf();
  void sleep(unsigned long interval);
  void handleActuators(JsonObject &reported);
  void readSensors(JsonObject &reported);
  void readBoardData(JsonObject &reported);
  void sendSavedMessages();
  void sendAllConfig();
  bool sendConfig(const char *moduleName, const char *filePath);
  void sendPublicIP();
  void startAP();
  void mqttProblem();
  void spiffsProblem();
  void messageSent();
  unsigned long secondsToNextLog();
  bool shouldLog();
  void printMqttState(int state);
  int mqttConnect();
  bool mqttPublish(String data);
  bool mqttListen();
  void mqttCallback(char *topic, byte *payload, unsigned int length);
  bool mqttsConfig();
  static int b64decode(String b64Text, uint8_t *output);
  void mqttsConvert(String cert);
  void updateFirmware(String firmwareVersion, String firmwareUrl, String firmwareUrlFingerprint);
  void tryFirmwareUpdate();

private:
  CoolBoardSensors coolBoardSensors;
  CoolBoardLed led;
  CoolTime rtc;
  CoolWifi *wifiManager = new CoolWifi;
  Jetpack jetPack;
  Irene3000 irene3000;
  ExternalSensors *externalSensors = new ExternalSensors;
  CoolBoardActuator onBoardActuator;
  CoolPubSubClient *mqttClient = new CoolPubSubClient;
  WiFiClientSecure *wifiClient = new WiFiClientSecure;
  bool ireneActive = false;
  bool jetpackActive = false;
  bool externalSensorsActive = false;
  bool sleepActive = true;
  bool manual = false;
  unsigned long logInterval = 3600;
  unsigned long previousLogTime = 0;
  String mqttId;
  String mqttServer;
  String mqttInTopic;
  String mqttOutTopic;
};

#endif
