#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

#define WLAN_SSID       "sourav"
#define WLAN_PASS       "12345678"

#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883                  

#define AIO_USERNAME    "Sourav1999"
#define AIO_KEY         "ab14ebdd4195486cbb064751292b1ba4"

#define pir D2
#define led D4
#define buzz D3/////////////////FOR BUZZER INSTEAD OF HIGH PUT LOW..........AS USED RELAY......................
#define b1 D6
#define b2 D7
#define b3 D8
int  pirstate=0;
int  data1;   
int  data2;
int  data3;
int  password;
WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);
//////////////////////////////////////////////////////...........................////////////////////////////////////////////////////////

Adafruit_MQTT_Publish sensor_value = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/button");///////////change feeds name////////////
Adafruit_MQTT_Publish pass = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/password");///////////change feeds name////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MQTT_connect();
void buzzer();
void datain();


void setup() {
  Serial.begin(9600);
  ///////////////////////////////////////////////////.............../////////////////////////////////////////////
 
  pinMode(pir,INPUT);
  pinMode(led,OUTPUT);
  pinMode(buzz,OUTPUT);
  pinMode(b1,INPUT);
  pinMode(b2,INPUT);
  pinMode(b3,INPUT);

  
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////
  WiFi.begin(WLAN_SSID, WLAN_PASS);
  delay(5000);
  //Serial.println(WiFi.localIP());
  }

  

void loop() {
   MQTT_connect();
  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(5000))) {   
  }
////////////////////////////////////////////////////..................../////////////////////////////////////////////
   int  val=digitalRead(pir);
   
   int lock=0;//////////////state of password//////////////////////////
   pass.publish(0);
   
  Serial.println(val);
  data1=digitalRead(b1);    
  data2=digitalRead(b2);
  data3=digitalRead(b3);
  
 
  if(val == HIGH){
      Serial.println("Motion Detected");
      while(password != HIGH){
               sensor_value.publish(1);
               buzzer();
               digitalWrite(led,HIGH);
               datain();
         }
         //delay(30000);
        // lock=1;
      Serial.println("UNLOCKED!!!");
      digitalWrite(buzz,HIGH);
      sensor_value.publish(0);
      digitalWrite(led,LOW);
      
      delay(30000);
      Serial.println("ACTIVE!!!!!!!!!!!!!");
      password=LOW;
      return;
       }
  else{
      digitalWrite(led,LOW);
      digitalWrite(buzz,HIGH);
      sensor_value.publish(0);
       } 

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
}


void buzzer(){
   digitalWrite(buzz,HIGH);
   delay(500);
   digitalWrite(buzz,LOW);
   delay(900);
}


void datain(){
      data1=digitalRead(b1);    
      data2=digitalRead(b2);
      data3=digitalRead(b3);
      password = data2;
      if(data1==HIGH)   {pass.publish(1); Serial.println("data sent = 1");}
      if(data2==HIGH)   {pass.publish(2); Serial.println("data sent = 2");}
      if(data3==HIGH)   {pass.publish(3); Serial.println("data sent = 3");}
  
}


void MQTT_connect() {
  int8_t ret;
  if (mqtt.connected()) {
    return;
  }

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) { 
       mqtt.disconnect();
       delay(5000);  // wait 5 seconds
       retries--;
       if (retries == 0) {
         while (1);
       }
  }
}
