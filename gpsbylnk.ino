//website name:https://how2electronics.com/real-time-gps-tracker-using-esp8266-blynk-with-maps/#google_vignette



#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
 
static const int RXPin = 4, TXPin = 5;   // GPIO 4=D2(conneect Tx of GPS) and GPIO 5=D1(Connect Rx of GPS
static const uint32_t GPSBaud = 9600; //if Baud rate 9600 didn't work in your case then use 4800
 
TinyGPSPlus gps;      // The TinyGPS++ object
WidgetMap myMap(V0);  // V0 for virtual pin of Map Widget
 
SoftwareSerial mygps(RXPin, TXPin);  // The serial connection to the GPS device
 
BlynkTimer timer;
 
float latitude;     //Storing the Latitude
float longitude;    //Storing the Longitude
float velocity;     //Variable  to store the velocity
float sats;         //Variable to store no. of satellites response
String bearing;     //Variable to store orientation or direction of GPS
 
char auth[] = "********************";            //Blynk Authentication Token
char ssid[] = "********************";            // WiFi SSID
char pass[] = "********************";            // WiFi Password
 
//unsigned int move_index;         // moving index, to be used later
unsigned int move_index = 1;       // fixed location for now
 
 
void setup()
{
  Serial.begin(115200);
  Serial.println();
  mygps.begin(GPSBaud);
  Blynk.begin(auth, ssid, pass);
  timer.setInterval(5000L, checkGPS); // every 5s check if GPS is connected, only really needs to be done once
}
 
void checkGPS()
{
  if (gps.charsProcessed() < 10)
  {
    Serial.println(F("No GPS detected: check wiring."));
    Blynk.virtualWrite(V3, "GPS ERROR");  // Value Display widget  on V3 if GPS not detected
  }
}
 
void loop()
{
  while (mygps.available() > 0)
  {
    // sketch displays information every time a new sentence is correctly encoded.
    if (gps.encode(mygps.read()))
      displayInfo();
  }
  Blynk.run();
  timer.run();
}
 
 
void displayInfo()
{
  if (gps.location.isValid() )
  {
    sats = gps.satellites.value();       //get number of satellites
    latitude = (gps.location.lat());     //Storing the Lat. and Lon.
    longitude = (gps.location.lng());
    velocity = gps.speed.kmph();         //get velocity
    bearing = TinyGPSPlus::cardinal(gps.course.value());     // get the direction
 
    Serial.print("SATS:  ");
    Serial.println(sats);  // float to x decimal places
    Serial.print("LATITUDE:  ");
    Serial.println(latitude, 6);  // float to x decimal places
    Serial.print("LONGITUDE: ");
    Serial.println(longitude, 6);
    Serial.print("SPEED: ");
    Serial.print(velocity);
    Serial.println("kmph");
    Serial.print("DIRECTION: ");
    Serial.println(bearing);
 
    Blynk.virtualWrite(V1, String(latitude, 6));
    Blynk.virtualWrite(V2, String(longitude, 6));
    Blynk.virtualWrite(V3, sats);
    Blynk.virtualWrite(V4, velocity);
    Blynk.virtualWrite(V5, bearing);
    myMap.location(move_index, latitude, longitude, "GPS_Location");
  }
  Serial.println();
}
