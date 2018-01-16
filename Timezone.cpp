#include "ESP8266WiFi.h"
#include <JsonStreamingParser.h>
#include "WsClient.h"
#include "TimeSpace.h"
#include "UserConfig.h"


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

#ifdef DEBUG_LOG
    Serial.println("URL = " + url);
#endif

    if (httpGet(url) && skipResponseHeaders())
    {
      JsonStreamingParser parser;
      parser.setListener(this);
      char c;

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
        c = client.read();

#ifdef DEBUG_LOG
        Serial.print(c);
#endif
        parser.parse(c);

        // Improves reliability from ESP version 2.4.0
        yield();
      }
    }
    else
    {
      // Get failed
#ifdef DEBUG_LOG
      Serial.println("get failed");
#endif
      return false;
    }
  }
  else
  {
    // Could not connect
#ifdef DEBUG_LOG
    Serial.println("Could not connect");
#endif
    return false;
  }

  disconnect();

  return true;
}

void Timezone::whitespace(char c) {
#ifdef DEBUG_LOG
  Serial.println("whitespace");
#endif
}

void Timezone::startDocument() {
#ifdef DEBUG_LOG
  Serial.println("start document");
#endif
}


void Timezone::key(String key) {
#ifdef DEBUG_LOG
  Serial.println("key: " + key);
#endif
  currentKey = String(key);
}

void Timezone::value(String value)
{
#ifdef DEBUG_LOG
  Serial.println("value: " + value);
#endif
  if (currentKey == F("dst"))
  {
    dst = value.toInt();
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
#ifdef DEBUG_LOG
  Serial.println("end array. ");
#endif
}

void Timezone::endObject() {
#ifdef DEBUG_LOG
  Serial.println("end object. ");
#endif
}

void Timezone::endDocument() {
#ifdef DEBUG_LOG
  Serial.println("end document. ");
#endif
}

void Timezone::startArray() {
#ifdef DEBUG_LOG
  Serial.println("start array. ");
#endif
}

void Timezone::startObject() {
#ifdef DEBUG_LOG
  Serial.println("start object. ");
#endif
}

String Timezone::getTimeZoneId()
{
  return timeZoneId;
}

String Timezone::getTimeZoneName()
{
  return timeZoneName;
}

bool Timezone::isDst()
{
  return dst;
}

int Timezone::getUtcOffset()
{
  return utcOffset;
}

