#define BLYNK_TEMPLATE_ID "TMPL3RrkB4s1z"/home/ramoji-naidu/Downloads/FALL_DETECTION_project/Falldetection_adv/Falldetection_adv.ino
#define BLYNK_TEMPLATE_NAME "FALL DETECTION"
#define BLYNK_AUTH_TOKEN "tGog9ioGsz364cWjJDsbIQ_1Csu0XnsJ"



#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <Wire.h>
#include <HardwareSerial.h>



#define RXD 16
#define TXD 15

char ssid[] = "YourNetworkName";
char pass[] = "YourPassword";

HardwareSerial MYserial( 1 );


BLYNK_CONNECTED() {

  Blynk.virtualWrite(V4,"Good");                                       //send message while board connected to Blynk cloud

}

void setup()
{

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);                      //initialising blynk connection
  MYserial.begin(115200, SERIAL_8N1, RXD, TXD);

}

void loop()
{
  if(MYserial.available()){
    float values[6] = MYserial.read();
    Blynk.virtualWrite(V0, values[0]);
    Blynk.virtualWrite(V1, values[1]);
    Blynk.virtualWrite(V5, values[2]);
    Blynk.virtualWrite(V2, values[3]);
    Blynk.virtualWrite(V3, values[4]);
    Blynk.virtualWrite(V6, values[5]);
    Blynk.virtualWrite(V4,"Fall Detected");
    while(1); 

  }




  delay(1000);

  Blynk.run();
}


