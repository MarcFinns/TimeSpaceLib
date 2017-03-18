//#include <ESP8266HTTPclient.h>
#include <ArduinoJson.h>
#include "ESP8266WiFi.h"
#include <NtpClientLib.h>         // http://github.com/gmag11/NtpClient
#include <JsonStreamingParser.h>
#include "UserConfig.h"
#include "TimeSpace.h"
#include "Wsclient.h"

// **********************************************************
//   Step 3 - Geocoding
//   Get Location name from latitude and longitude
// **********************************************************

bool Geocode::acquire(double latitude, double longitude)
{
  //Connect to the client and make the api call
  if (httpConnect())
  {
    // Concatenate url and key
    String url = FPSTR(geoCodeURL);
    url += F("&lat=");
    url += String(latitude, 8);
    url += F("&lng=");
    url += String(longitude, 8);
    url += F("&username=" );
    url += FPSTR(geoCodeUsername);

    // Serial.println("URL = " + url);

    if (httpGet(url) && skipResponseHeaders())
    {
      // Invalidate current values 
      locality = FPSTR(empty);
      country = FPSTR(empty);
      countryCode = FPSTR(empty);

      // Allow for slow server...
      int retryCounter = 0;
      while (!client.available())
      {
        delay(1000);
        retryCounter++;
        if (retryCounter > 10)
        {
          return false;
        }
      }

      while (client.available())
      {
        JsonStreamingParser parser;
        parser.setListener(this);
        char c;
        int size = 0;

        while ((size = client.available()) > 0)
        {
          c = client.read();
          // Serial.print(c);
          parser.parse(c);
        }
      }
    }
    else
      // Get failed
      return false;
  }
  else
    // Could not connect
    return false;

  disconnect();

  return true;
}

void Geocode::whitespace(char c) {
  // Serial.println("whitespace");
}

void Geocode::startDocument() {
  //   // Serial.println("start document");
}

void Geocode::key(String key)
{
  // Serial.println("key: " + key);
  currentKey = key;
}

void Geocode::value(String value)
{
  // Serial.println("value: " + value);
  if (currentKey == F("name"))
  {
    locality = value;
  }
  else if (currentKey == F("countryName"))
  {
    country = value;
  }
  else if (currentKey == F("countryCode"))
  {
    countryCode = value;
  }
}


void Geocode::endArray() {
  // Serial.println("end array. ");
}

void Geocode::endObject() {
  //   // Serial.println("end object. ");
}

void Geocode::endDocument() {
  //   // Serial.println("end document. ");
}

void Geocode::startArray() {
  //   // Serial.println("start array. ");
}

void Geocode::startObject() {
  //   // Serial.println("start object. ");
}

String Geocode::getLocality()
{
  return locality;
}

String Geocode::getCountry()
{
  return country;
}

String Geocode::getCountryCode()
{
  return countryCode;
}

