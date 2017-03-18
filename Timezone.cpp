#include "ESP8266WiFi.h"
#include <JsonStreamingParser.h>
#include "WsClient.h"
#include "TimeSpace.h"
#include "UserConfig.h"

//extern NTPClient NTP;


bool Timezone::acquire(double latitude, double longitude)
{
  /**********************************************************
     Step 2 - Timezone
     Get Timezone from latitude and longitude
   **********************************************************/

  //Connect to the client and make the api call
  if (httpConnect())
  {
    // Concatenate url and key
    String url = FPSTR(timeZoneURL);
    url += F("lat=");
    url += String(latitude, 6);
    url += F("&lng=") ;
    url += String(longitude, 6) ;
    url += F("&key=") ;
    url += FPSTR(timeZoneKey);

    // Serial.println("URL = " + url);

    if (httpGet(url) && skipResponseHeaders())
    {
      JsonStreamingParser parser;
      parser.setListener(this);
      char c;
      int size = 0;

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
      
      while ((size = client.available()) > 0)
      {
        c = client.read();
        // Serial.print(c);
        parser.parse(c);
      }
    }
    else
    {
      // Get failed
      // Serial.println("get failed");
      return false;
    }
  }
  else
  {
    // Could not connect
    // Serial.println("Could not connect");
    return false;
  }

  disconnect();

  return true;
}

void Timezone::whitespace(char c) {
  // Serial.println("whitespace");
}

void Timezone::startDocument() {
  // Serial.println("start document");
}


void Timezone::key(String key) {
  // Serial.println("key: " + key);
  currentKey = String(key);
}

void Timezone::value(String value)
{
  // Serial.println("value: " + value);
  if (currentKey == F("dst"))
  {
    dstOffset = value.toInt();
  }
  else if (currentKey == F("gmtOffset"))
  {
    utcOffset = value.toInt();
  }
  else if (currentKey == F("abbreviation"))
  {
    timeZoneId = value;
  }
  else if (currentKey == F("zoneName"))
  {
    timeZoneName = value;
  }
}

void Timezone::endArray() {
  // Serial.println("end array. ");
}

void Timezone::endObject() {
  // Serial.println("end object. ");
}

void Timezone::endDocument() {
  // Serial.println("end document. ");
}

void Timezone::startArray() {
  // Serial.println("start array. ");
}

void Timezone::startObject() {
  // Serial.println("start object. ");
}

String Timezone::getTimeZoneId()
{
  return timeZoneId;
}

String Timezone::getTimeZoneName()
{
  return timeZoneName;
}

int Timezone::getDstOffset()
{
  return dstOffset;
}

int Timezone::getUtcOffset()
{
  return utcOffset;
}

