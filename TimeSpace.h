#pragma once

#include "ESP8266WiFi.h"
#include "JsonListener.h"
#include "Wsclient.h"
#include "UserConfig.h"

class Timezone : public JsonListener, public WsClient
{
  public:
    Timezone()
    {
#ifdef DEBUG_LOG
      Serial.println("Timezone constructor");
#endif
      hostName = FPSTR(timeZoneHost);
    };

    ~Timezone()
    {
#ifdef DEBUG_LOG
      Serial.println("Timezone destructor");
#endif
    };

    bool acquire(double latitude, double longitude);
    virtual void whitespace(char c);
    virtual void startDocument();
    virtual void key(String key);
    virtual void value(String value);
    virtual void endArray();
    virtual void endObject();
    virtual void endDocument();
    virtual void startArray();
    virtual void startObject();
    String getTimeZoneId();
    String getTimeZoneName();
    bool isDst();
    int getUtcOffset();

  private:
    String currentKey;
    String currentParent;
    bool dst;
    int utcOffset;
    String timeZoneId;
    String timeZoneName;
};

class Geolocate : public JsonListener, public WsClient
{
  public:
    Geolocate() {
#ifdef DEBUG_LOG
      Serial.println("Geolocate constructor");
#endif
      hostName = FPSTR(geoLocateHost);
    };
    ~Geolocate() {
#ifdef DEBUG_LOG
      Serial.println("Geolocate destructor");
#endif
    };

    bool acquire();
    virtual void whitespace(char c);
    virtual void startDocument();
    virtual void key(String key);
    virtual void value(String value);
    virtual void endArray();
    virtual void endObject();
    virtual void endDocument();
    virtual void startArray();
    virtual void startObject();
    double getLatitude();
    double getLongitude();

  private:
    String encodeBase64(char* bytes_to_encode, unsigned int in_len);
    double latitude    = 0.0;
    double longitude   = 0.0;
    String currentKey;
    String currentParent;
    String base64_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
};


class Geocode : public JsonListener, public WsClient
{
  public:
    Geocode() {
#ifdef DEBUG_LOG
      Serial.println("Geocode constructor");
#endif
      hostName = FPSTR(geoCodeHost);
    };
    ~Geocode() {
#ifdef DEBUG_LOG
      Serial.println("Geocode destructor");
#endif
    };

    bool acquire(double latitude, double longitude);
    String getLocality();
    String getCountry();
    String getCountryCode();


    virtual void whitespace(char c);
    virtual void startDocument();
    virtual void key(String key);
    virtual void value(String value);
    virtual void endArray();
    virtual void endObject();
    virtual void endDocument();
    virtual void startArray();
    virtual void startObject();

  private:
    String currentKey;
    String locality;
    String country;
    String countryCode;


};

