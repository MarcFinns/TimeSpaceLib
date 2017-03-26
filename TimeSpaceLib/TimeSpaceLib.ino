#include <NtpClientLib.h>         // http://github.com/gmag11/NtpClient
#include <ESP8266WiFi.h>
#include "TimeSpace.h"

// Wifi
static char myssid[] = "YOUR-SSID";          // your network SSID (name)
static char mypass[] = "YOUR-PASSWORD";          // your network password

// NTP server
const char PROGMEM ntpServerName[] = "pool.ntp.org";

// Values acquired from web services
double latitude;
double longitude;
bool dst;
int utcOffset;
String timeZoneId;
String timeZoneName;
String locality;
String countryCode;
String country;

void setup()
{
  Serial.begin(115200);
  Serial.println("START - Free Heap: " + String(ESP.getFreeHeap()));

  // Connect to WiFi
  Serial.print("Connecting to ");
  Serial.println(myssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(myssid, mypass);

  // Wait for connection...
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println(".");
  Serial.println("Connected to network " + String(WiFi.SSID()) + " with address " + String(WiFi.localIP().toString()));

  Serial.println("before timespace - Free Heap: " + String(ESP.getFreeHeap()));
  initTimeSpace();
  Serial.println("before NTP - Free Heap: " + String(ESP.getFreeHeap()));
  initNTP();

  Serial.println("END - Free Heap: " + String(ESP.getFreeHeap()));
}

void loop()
{

  Serial.println( "======== TIME ==================");
  Serial.print(NTP.getTimeDateString()); Serial.print(" ");
  Serial.println(NTP.isSummerTime() ? "Summer Time. " : "Winter Time. ");

  Serial.println( "======== TIME ZONE ==================");
  Serial.println( "UTC Offset = " + String(utcOffset));
  Serial.println( "DST = " + String(dst));
  Serial.println( "Time Zone ID = " + timeZoneId);
  Serial.println( "Time Zone Name = " + timeZoneName);

  Serial.println( "======== COORDINATES ==================");
  Serial.println( "Latitude = " + String(latitude));
  Serial.println( "Longitude = " + String(longitude));

  Serial.println( "======== ADDRESS ==================");
  Serial.println( "Locality = " + locality);
  Serial.println( "Country Code = " + countryCode);
  Serial.println("country = " + country);

  Serial.println( "==========================");
  Serial.println("Loop - Free Heap: " + String(ESP.getFreeHeap()));

  while (1)
  {
    delay(5000);
  }
}

void initNTP()
{

  // Setup NTP
  NTP.onNTPSyncEvent([](NTPSyncEvent_t event)
  {
    Serial.println();
    Serial.println( "Sync event");
    if (event)
    {
      if (event == noResponse)
        Serial.println( "NTP server not reachable");
      else if (event == invalidAddress)
        Serial.println("Invalid NTP server address");
    }
    else
    {
      Serial.println("Got NTP time - " + NTP.getTimeDateString(NTP.getLastNTPSync()));
    }
  });


  // Get NTP time according to local time zone / DST
  Serial.println("4 ------- Sincronising time via NTP on local time zone...");
  
  // NOTE: UTCOffset already contains DST offset!
  NTP.begin(FPSTR(ntpServerName), utcOffset / (3600 * (1 + dst)), dst);
  NTP.setInterval(5, 1800);

  // Wait until NTP time is synchronised
  Serial.println(NTP.getTimeDateString());
  while (NTP.getLastNTPSync() == 0)
  {
    Serial.print(".");
    delay(500);
    NTP.getTimeDateString();
  }
}

void initTimeSpace()
{

  //  Geolocation -  Acquire coordinates

  Serial.println("1 ------- Retrieving coordinates...");
  Geolocate geolocate;

  geolocate.acquire();

  // Save variables
  latitude = geolocate.getLatitude();
  longitude = geolocate.getLongitude();


  // Acquire timezone and daylight saving
  Serial.println("2 ----------- Retrieving timezone...");

  Timezone timezone;

  timezone.acquire(latitude, longitude);

  // Save variables
  utcOffset = timezone.getUtcOffset();
  dst = timezone.isDst();
  timeZoneId = timezone.getTimeZoneId();
  timeZoneName = timezone.getTimeZoneName();


  // Acquire location name

  Serial.println("3 ----------- Acquiring geo address...");
  Geocode geocode;
  geocode.acquire(latitude, longitude);

  // Save variables
  locality = geocode.getLocality();
  country = geocode.getCountry();
  countryCode = geocode.getCountryCode();

}

