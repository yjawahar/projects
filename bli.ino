//#include <Wire.h>
#include <MPU6050.h>
#include <TinyGPS++.h>
#include <math.h>
#include <SoftwareSerial.h>
#include <Servo.h>

Servo myservo;  // Create a servo object to control a servo motor
int angle = 0;  // Initialize the angle variable


const int g1 = 2; // Define the pin where the button is connected
int gear1 = 0; 
const int g2 = 3; // Define the pin where the button is connected
int gear2 = 0; 
const int g3 = 4; // Define the pin where the button is connected
int gear3 = 0; 
const int g4 = 5; // Define the pin where the button is connected
int gear4 = 0; 
const int g5 = 10; // Define the pin where the button is connected
int gear5 = 0; 

SoftwareSerial neogps(8, 9);  // SoftwareSerial object for GPS communication
TinyGPSPlus gps;
const String EMERGENCY_PHONE = "8919725104";
#define rxPin 6
#define txPin 7
byte updateflag=0,upflag=0;
SoftwareSerial sim800(rxPin, txPin);

MPU6050 mpu;
 
String sms_status, sender_number, received_date, msg;
String latitude, longitude;

#define sos 11

boolean impact_detected = false;
unsigned long time1;
unsigned long impact_time;
unsigned long alert_delay = 30000;
 int16_t accelerometerX, accelerometerY, accelerometerZ;
  int16_t a1,a2,a3;
  int magnitude=0;
 

void setup()
{
  pinMode(g1, INPUT); 
  pinMode(g2, INPUT); 
  pinMode(g3, INPUT); 
  pinMode(g4, INPUT); 
  pinMode(g5, INPUT); 
 myservo.attach(11); 
 Serial.begin(9600);
   delay(10);
 

  pinMode(sos,INPUT_PULLUP);
 
  Wire.begin();
  mpu.initialize();
  sms_status = "";
  sender_number = "";
  received_date = "";
  msg = "";
 sim800.begin(9600);
 delay(1000);
  sim800.println("AT");
  delay(1000);
  sim800.println("ATE1");
  delay(1000);
  sim800.println("AT+CPIN?");
  delay(1000);
  sim800.println("AT+CMGF=1");
  delay(1000);
  sim800.println("AT+CNMI=1,1,0,0,0");
  delay(1000);
  time1 = micros();
 neogps.begin(9600);
}

   void loop() {
gear1 = digitalRead(g1);
gear2 = digitalRead(g2);
gear3 = digitalRead(g3);
gear4 = digitalRead(g4);
gear5 = digitalRead(g5);

 if (gear1 == HIGH) {

for (angle = 0; angle <= 90; angle += 1) { // Increase angle from 0 to 180 degrees
    myservo.write(angle);  // Move the servo to the specified angle
    delay(15);  // Wait for the servo to reach the angle
  }
  
  delay(100); // Wait for a second
  
  for (angle = 90; angle >= 0; angle -= 1) { // Decrease angle from 180 to 0 degrees
    myservo.write(angle);  // Move the servo to the specified angle
    delay(15);  // Wait for the servo to reach the angle
  }
  
  delay(1000);

    Serial.println("gear1 is applied");

  } 
  else if (gear2 == HIGH) {
for (angle = 0; angle <= 90; angle += 1) { // Increase angle from 0 to 180 degrees
    myservo.write(angle);  // Move the servo to the specified angle
    delay(15);  // Wait for the servo to reach the angle
  }
  
  delay(100); // Wait for a second
  
  for (angle = 90; angle >= 0; angle -= 1) { // Decrease angle from 180 to 0 degrees
    myservo.write(angle);  // Move the servo to the specified angle
    delay(15);  // Wait for the servo to reach the angle
  }
  

    Serial.println("gear2 is applied");
  }



 else if (gear3 == HIGH) {
for (angle = 0; angle <= 90; angle += 1) { // Increase angle from 0 to 180 degrees
    myservo.write(angle);  // Move the servo to the specified angle
    delay(15);  // Wait for the servo to reach the angle
  }
  
  delay(100); // Wait for a second
  
  for (angle = 90; angle >= 0; angle -= 1) { // Decrease angle from 180 to 0 degrees
    myservo.write(angle);  // Move the servo to the specified angle
    delay(15);  // Wait for the servo to reach the angle
  }
  

    Serial.println("gear3 is applied");
  }

 else if (gear5 == HIGH) {
for (angle = 0; angle <= 90; angle += 1) { // Increase angle from 0 to 180 degrees
    myservo.write(angle);  // Move the servo to the specified angle
    delay(15);  // Wait for the servo to reach the angle
  }
  
  delay(100); // Wait for a second
  
  for (angle = 90; angle >= 0; angle -= 1) { // Decrease angle from 180 to 0 degrees
    myservo.write(angle);  // Move the servo to the specified angle
    delay(15);  // Wait for the servo to reach the angle
  }
  

    Serial.println("gear5 is applied");
  }

 else if (gear4 == HIGH) {
for (angle = 0; angle <= 90; angle += 1) { // Increase angle from 0 to 180 degrees
    myservo.write(angle);  // Move the servo to the specified angle
    delay(15);  // Wait for the servo to reach the angle
  }
  
  delay(100); // Wait for a second
  
  for (angle = 90; angle >= 0; angle -= 1) { // Decrease angle from 180 to 0 degrees
    myservo.write(angle);  // Move the servo to the specified angle
    delay(15);  // Wait for the servo to reach the angle
  }
  

    Serial.println("gear4 is applied");
  }

 mpu.getAcceleration(&accelerometerX, &accelerometerY, &accelerometerZ);
  if(digitalRead(sos)==0)
  {
    upflag=10;
  }
updateflag =Impact( accelerometerX,  accelerometerY, accelerometerZ);


  if ((updateflag>0)||(upflag>0))
  {
    updateflag = 0;
     upflag = 0;
 
    Serial.println("accident occured");
    Serial.print("Magnitude:");
    Serial.println(magnitude);
   
  impact_detected = true;
    getGps();
    
   
    impact_time = millis();
 
 
  }
  if (impact_detected == true)
  {
    if (millis() - impact_time >= alert_delay) {

     
      makeCall();
      getGps();
      delay(1000);
      sendAlert();
      impact_detected = false;
      impact_time = 0;
    }
  }
 

  while (sim800.available()) {
    parseData(sim800.readString());
  }
  while (Serial.available())  {
    sim800.println(Serial.readString());
  }
   }
 
byte Impact(int16_t accelerometerX, int16_t accelerometerY,int16_t accelerometerZ)
{ 
   a1=accelerometerX/1000;
   a2=accelerometerY/1000;
   a3=accelerometerZ/1000;
   
   Serial.println(a1);
    Serial.println(a2);
     Serial.println(a3);
 
  if(((((a2)>=20)&&((a2)<24))||(((a2)>=11)&&((a2)<10)))||((((a1)>=9)&&((a1)<15))||(((a1)>-15)&&((a1)<-14)))||((a3>7)&&(a3<10)))
   {
    // Activate the buzzer
    magnitude=sqrt((a1)*(a1)+(a2)*(a2)+(a3)*(a3));
    updateflag =10;
  }
return(updateflag);
}
 
 
void parseData(String buff) {
  Serial.println(buff);
 
  unsigned int len, index;
  index = buff.indexOf("\r");
  buff.remove(0, index + 2);
  buff.trim();
  if (buff != "OK") {
    index = buff.indexOf(":");
    String cmd = buff.substring(0, index);
    cmd.trim();
 
    buff.remove(0, index + 2);
    if (cmd == "+CMTI") {
      index = buff.indexOf(",");
      String temp = buff.substring(index + 1, buff.length());
      temp = "AT+CMGR=" + temp + "\r";
      sim800.println(temp);
    }
    else if (cmd == "+CMGR") {
      if (buff.indexOf(EMERGENCY_PHONE) > 1) {
        buff.toLowerCase();
        if (buff.indexOf("get gps") > 1) {
          getGps();
          String sms_data;
          sms_data = "GPS Location Data\r";
          sms_data += "http://maps.google.com/maps/place/";
          sms_data += latitude + "," + longitude;
 
          sendSms(sms_data);
        }
      }
    }
  }
  else {
  }
}


void getGps()
{
  // Can take up to 60 seconds
  boolean newData = false;
  for (unsigned long start = millis(); millis() - start < 2000;) {
 
    while (neogps.available()) {
      if (gps.encode(neogps.read())) {
        newData = true;
        break;
      }
    }
  }
 
  if (true)
  {
    latitude = String(gps.location.lat(), 6);
    longitude = String(gps.location.lng(), 6);
    newData = false;
  }
  else {
    Serial.println("No GPS data is available");
    latitude = "";
    longitude = "";
    
  }
 
  Serial.print("Latitude= "); Serial.println(latitude);
  Serial.print("Lngitude= "); Serial.println(longitude);
}
 
 
void sendAlert()
{
  String sms_data;
  sms_data = "Accident Alert!!\r";
  sms_data += "http://maps.google.com/maps/place/";
  sms_data += latitude + "," + longitude;
 
  sendSms(sms_data);
}
 
void makeCall()
{
  Serial.println("calling....");
  sim800.println("ATD" + EMERGENCY_PHONE + ";");
  delay(20000); //20 sec delay
  //1 sec delay
}
 
void sendSms(String text)
{
  //return;
  sim800.print("AT+CMGF=1\r");
  delay(1000);
  sim800.print("AT+CMGS=\"" + EMERGENCY_PHONE + "\"\r");
  delay(1000);
  sim800.print(text);
  delay(100);
  sim800.write(0x1A);
  delay(1000);
  Serial.println("SMS Sent Successfully.");
}
 
boolean SendAT(String at_command, String expected_answer, unsigned int timeout) {
 
  uint8_t x = 0;
  boolean answer = 0;
  String response;
  unsigned long previous;
  while ( sim800.available() > 0) sim800.read();
 
  sim800.println(at_command);
 
  x = 0;
  previous = millis();
 
  do {
    if (sim800.available() != 0) {
      response += sim800.read();
      x++;
      if (response.indexOf(expected_answer) > 0) {
        answer = 1;
        break;
      }
    }
  }
  while ((answer == 0) && ((millis() - previous) < timeout));
  Serial.println(response);
  return answer;
}


