#include "DHT.h"
#define DHTPIN 8 
#define DHTTYPE DHT11 
DHT dht(DHTPIN,DHTTYPE);
#include <Wire.h>
#include <LiquidCrystal.h>
#include <OneWire.h>
#include <Servo.h>
#include <DallasTemperature.h>
#define ONE_WIRE_BUS 12
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
const int pingPin =A3; // Trigger Pin of Ultrasonic Sensor
const int echoPin =A2; // Echo Pin of Ultrasonic Sensor
long duration;
int distance;
LiquidCrystal lcd(2, 3, 4, 5, 6, 7);
const int bz=13;
int kk=0;
Servo feed;
int ts=A0;
int is=11;
int mp1=9;
int mp2=10;
void setup() {   

    Wire.begin();
   // Serial.begin(9600);
    Serial.begin(115200);
    pinMode(pingPin, OUTPUT);
pinMode(echoPin, INPUT);
    sensors.begin();
    lcd.begin(16, 2);
    lcd.setCursor(0,0);
    lcd.print("AQUA MNTRNG");
  //  feed.attach(10);
    pinMode(mp1,OUTPUT);
     dht.begin();
    pinMode(mp2,OUTPUT);
    pinMode(bz,OUTPUT);
     wifi_init();
    feed.write(0);
    digitalWrite(bz,0);
    digitalWrite(mp1,0);
    digitalWrite(mp2,0);
    pinMode(ts,INPUT);
    pinMode(is,INPUT);
    delay(2000);
}

void loop() {
  delay(200);
  digitalWrite(pingPin, LOW);
delayMicroseconds(2);
digitalWrite(pingPin, HIGH);
delayMicroseconds(10);
digitalWrite(pingPin, LOW);
duration = pulseIn(echoPin, HIGH);
distance=duration*0.034/2;
Serial.println(distance);  
int tbval=analogRead(ts)/20;
  //  int pval=analogRead(ps)/55;
int irval=1-digitalRead(is);
  //  int cval=1023-analogRead(cd);
sensors.requestTemperatures();
int tval = sensors.getTempCByIndex(0);
  int t = dht.readTemperature();
  int h=dht.readHumidity();
    lcd.clear();
    lcd.print("T:"+ String(tval) + " TB:"+ String(tbval) + " OT:"+ String(t));
    lcd.setCursor(0,1);
    lcd.print("IR:" + String(irval) + " H:" + String(h) + " 
    
    L:" + String(distance));
    if(distance<20)
    {
      digitalWrite(mp1,1);
      digitalWrite(mp2,0);
    }
    else
    {
     digitalWrite(mp1,0);
     digitalWrite(mp2,0); 
    }
    delay(300);
    if(kk>25)
    {
      kk=0;
    
    upload(tval,tbval,irval,distance,t,h);
}
kk=kk+1;
}


    void wifi_init()
 {
  Serial.println("AT+RST");
  delay(4000);
  Serial.println("AT+CWMODE=3");
  delay(4000);
  Serial.print("AT+CWJAP=");
  Serial.write('"');
  Serial.print("project"); // ssid/user name
  Serial.write('"');
  Serial.write(',');
  Serial.write('"');
  Serial.print("12345678"); //password
  Serial.write('"');
  Serial.println();
  delay(1000);
 }


void upload(int x, int y, int z,int p ,int q,int r) //ldr copied int to - x   and gas copied into -y
{
  
  String cmd = "AT+CIPSTART=\"TCP\",\"";
  cmd += "184.106.153.149"; // api.thingspeak.com
  cmd += "\",80";
  Serial.println(cmd);
  delay(1000);
  String getStr ="GET /update?api_key=7ISW600EH3UQVV7W&field1=";
  getStr += String(x);
  getStr +="&field2=";
  getStr += String(y);
  getStr +="&field3=";
  getStr += String(z);
  getStr +="&field4=";
  getStr += String(p);
  getStr +="&field5=";
  getStr += String(q);
    getStr +="&field6=";
  getStr += String(r);
  getStr += "\r\n\r\n";
  cmd = "AT+CIPSEND=";
  cmd += String(getStr.length());
  Serial.println(cmd);
  delay(1000);
  Serial.println(getStr);
}
