#pragma once

#define MAX_SSIDS 8

#define min(a,b) ((a)<(b)?(a):(b))

// Geolocate
const char PROGMEM  geoLocateHost[] = "api.mylnikov.org";
const char PROGMEM  geoLocateURL[] = "/wifi?v=1.1&search=";

// Timezone
const char PROGMEM  timeZoneHost[] = "api.timezonedb.com";
const char PROGMEM  timeZoneURL[] = "/v2/get-time-zone?format=json&by=position&";
const char PROGMEM  timeZoneKey[] = "YOUR-KEY-HERE";

// GeoCode
const char PROGMEM  geoCodeHost[] = "api.geonames.org";
const char PROGMEM  geoCodeURL[] = "/findNearbyPlaceNameJSON?";
const char PROGMEM  geoCodeUsername[] = "YOUR-USERNAME-HERE";

// Constants
const char PROGMEM empty[] = "";

