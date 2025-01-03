#define BLYNK_TEMPLATE_ID "TMPL3RrkB4s1z"
#define BLYNK_TEMPLATE_NAME "FALL DETECTION"
#define BLYNK_AUTH_TOKEN "tGog9ioGsz364cWjJDsbIQ_1Csu0XnsJ"
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
//#define LED_BUILTIN 


char ssid[] = "YourNetworkName";
char pass[] = "YourPassword";




const float fallThreshold = 2.0; 
const float gyroThreshold = 5.0; 
bool fallstatus = false;
Adafruit_MPU6050 mpu;

BLYNK_CONNECTED() {

  Blynk.logEvent("connection");                                       //send message while board connected to Blynk cloud
  //digitalWrite(LED_BUILTIN,HIGH);                                     // built in led will turn on

}

void setup()
{

  //pinMode(LED_BUILTIN,OUTPUT);                                    //set pinmode of built in led to output

  delay(200);                                                     //delay of 200 micro second

  if (!mpu.begin()) {
    Blynk.logEvent("mpu_not_found");                              //check for mpu6050 status and send  message if not connected 
  }

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);                      //initialising blynk connection
}

void loop()
{
  sensors_event_t accel, gyro, temp;
  if(mpu.getMotionInterruptStatus()) {

    mpu.getEvent(&accel, &gyro, &temp);

    float totalAcceleration = sqrt(pow(accel.acceleration.x, 2) + pow(accel.acceleration.y, 2) + pow(accel.acceleration.z, 2));
    float totalGyro = sqrt(pow(gyro.gyro.x, 2) + pow(gyro.gyro.y, 2) + pow(gyro.gyro.z, 2));

    if (totalAcceleration > fallThreshold && totalGyro > gyroThreshold && !fallstatus) {
      Blynk.logEvent("fall_detected");
      fallstatus = true;
    }
    else if (!(totalAcceleration > fallThreshold && totalGyro > gyroThreshold)) {
    fallstatus = false;
    }
  }

  if (!fallstatus) {
    Blynk.virtualWrite(V4,"Good");                                   //send present sensor data to blynk app
  }
  else
  {
    Blynk.virtualWrite(V4,"Fall Detected"); 
  }
  Blynk.virtualWrite(V0, accel.acceleration.x);
  Blynk.virtualWrite(V1, accel.acceleration.y);
  Blynk.virtualWrite(V5, accel.acceleration.z);
  Blynk.virtualWrite(V2,gyro.gyro.x);
  Blynk.virtualWrite(V3,gyro.gyro.y);
  Blynk.virtualWrite(V6,gyro.gyro.z);


  delay(1000);

  Blynk.run();
}


