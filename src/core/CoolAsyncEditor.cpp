#include "CoolAsyncEditor.h"
#include "ArduinoJson.h"
#include "CoolLog.h"
#include <FS.h>
#include <avr/pgmspace.h>
#include <user_interface.h>

CoolAsyncEditor::CoolAsyncEditor(const fs::FS &fs) : _fs(fs) {}

void CoolAsyncEditor::write(String patch, String data) {
  File tmp = _fs.open(patch, "w");
  tmp.write((const uint8_t *)data.c_str(), data.length());
  tmp.close();
}

bool CoolAsyncEditor::remove(String patch) { return (_fs.remove(patch)); }

void CoolAsyncEditor::reWriteWifi(String data) {
  DEBUG_VAR("New wifiConfig: ", data);
  this->write("/wifiConfig.json", data);
  INFO_LOG("wifi reset!");
}

bool CoolAsyncEditor::addNewWifi(String ssid, String pass) {
  DynamicJsonBuffer json;
  JsonObject &jsonBuf =
      json.parseObject(this->read("/wifiConfig.json").c_str());
  uint8_t wifiCount = jsonBuf.get<uint8_t>("wifiCount");
  DEBUG_VAR("Number of recorded networks: ", wifiCount);
  if (jsonBuf["wifiCount"].success()) {
    JsonObject &newWifi =
        jsonBuf.createNestedObject("Wifi" + String(wifiCount));
    jsonBuf["wifiCount"] = wifiCount + 1;
    newWifi["ssid"] = ssid;
    newWifi["pass"] = pass;
    String tmp;
    jsonBuf.printTo(tmp);
    DEBUG_VAR("New wifi Configuration: ", tmp);
    this->write("/wifiConfig.json", tmp);
    return (true);
  }
}

String CoolAsyncEditor::getSavedWifi(String index) {
  DynamicJsonBuffer json;
  JsonObject &jsonBuf =
      json.parseObject(this->read("/wifiConfig.json").c_str());
  if (jsonBuf["Wifi" + index].success()) {
    String tmp;
    JsonObject &jWifi = jsonBuf["Wifi" + String(index)];
    jWifi.printTo(tmp);
    DEBUG_VAR("Wifi requested: ", tmp);
    return (tmp);
  }
  return ("ERROR");
}

bool CoolAsyncEditor::connectToSavedWifi(String index) {
  DynamicJsonBuffer json;
  JsonObject &jsonBuf =
      json.parseObject(this->read("/wifiConfig.json").c_str());
  if (jsonBuf["Wifi" + index].success()) {
    String tmp;
    JsonObject &jWifi = jsonBuf["Wifi" + String(index)];
    WiFi.begin(jWifi.get<String>("ssid").c_str(), jWifi.get<String>("pass").c_str());
    DEBUG_VAR("Connection Status: ",this->waitForConnectResult());
  }
}


uint8_t CoolAsyncEditor::waitForConnectResult() {
  if (_connectTimeout == 0) {
    return WiFi.waitForConnectResult();
  } else {
    unsigned long start = millis();
    boolean keepConnecting = true;
    uint8_t status;
    while (keepConnecting) {
      status = WiFi.status();
      if (millis() > start + _connectTimeout) {
        keepConnecting = false;
      }
      if (status == WL_CONNECTED || status == WL_CONNECT_FAILED) {
        keepConnecting = false;
      }
    }
    return status;
  }
}


String CoolAsyncEditor::read(String patch) {
  File file = _fs.open(patch, "r");
  uint8_t linebuf[file.size()];
  if (file.size() > 0) {
    uint32_t idx;
    while (file.available()) {
      linebuf[0] = '\0';
      idx = 0;
      uint8_t lastChar;
      do {
        lastChar = file.read();
        linebuf[idx++] = lastChar;
      } while ((lastChar >= 0) && (idx < (file.size() + 1)));
      linebuf[idx - 1] = '\0';
    }
  }
  String buf = String((char *)linebuf);
  DEBUG_VAR("Async read From SPIFFS: ", buf);
  file.close();
  return (buf);
}

char *CoolAsyncEditor::readChars(String patch) {
  File file = _fs.open(patch, "r");
  uint8_t linebuf[file.size()];
  if (file.size() > 0) {
    uint32_t idx;
    while (file.available()) {
      linebuf[0] = '\0';
      idx = 0;
      uint8_t lastChar;
      do {
        lastChar = file.read();
        linebuf[idx++] = lastChar;
      } while ((lastChar >= 0) && (idx < (file.size() + 1)));
      linebuf[idx - 1] = '\0';
    }
  }
  DEBUG_VAR("Async read From SPIFFS: ", (char *)linebuf);
  file.close();
  return ((char *)linebuf);
}

String CoolAsyncEditor::getSdpConfig() {
  String friendlyName = "CoolBoard-" + this->getMAC();
  String ip = WiFi.localIP().toString();
  String xml = this->read("/description.xml");
  int outputLength = xml.length() + ip.length() + friendlyName.length() +
                     (this->getMAC().length() * 3) +
                     this->getFlashID().length() + 1;
  char buffer[outputLength];
  buffer[outputLength - 1] = (char)NULL;
  sprintf(buffer, xml.c_str(), ip.c_str(), friendlyName.c_str(),
          this->getMAC().c_str(), this->getMAC().c_str(),
          this->getUUID().c_str());
  DEBUG_VAR("UPnP descriptor: ", buffer);
  return String(buffer);
}

String CoolAsyncEditor::getFlashID() {
  return (String(ESP.getFlashChipId(), HEX));
}

String CoolAsyncEditor::getMAC() {
  String tempMAC = WiFi.macAddress();
  tempMAC.replace(":", "");
  return (tempMAC);
}

String CoolAsyncEditor::getUUID() {
  uint32_t chipId = ESP.getChipId();
  char uuid[SSDP_UUID_SIZE];
  sprintf(uuid, "38323636-4558-4dda-9188-cda0e6%02x%02x%02x",
          (uint16_t)((chipId >> 16) & 0xff), (uint16_t)((chipId >> 8) & 0xff),
          (uint16_t)chipId & 0xff);
  return (uuid);
}
