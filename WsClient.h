#pragma once

#include "ESP8266WiFi.h"

class WsClient
{
  public:
    WsClient()
    {
      // Serial.println("WsClient constructor");
    };

    ~WsClient()
    {
      // Serial.println("WsClient destructor");
    };
    
  public:
    String hostName;
    bool httpConnect();
    bool httpGet(String resource);
    bool skipResponseHeaders();
    void disconnect();

    WiFiClient client;
};
