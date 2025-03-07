/*
///////////////////////////////////////////////
**********COMMUNITY SERVICE PROJECT************
    
      project_name      : Fall-detection Using arduino and esp32
      project_guide     :
      project_members   :         Name                                   Roll-Number
                          1)
                          2)
                          3)
                          4)
                          5)

      branch            : Electronics And Communication Engineering
      course            : B-Tech
      year              : 

///////////////////////////////////////////////////
*/



#include <SoftwareSerial.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>


SoftwareSerial mySerial1(10, 11);           // Set up a new SoftwareSerial object with RX in digital pin 10 and TX in digital pin 11 for esp32
SoftwareSerial mySerial2(3,2);              // Set up a new SoftwareSerial object with RX in digital pin 03 and TX in digital pin 02 for GSM module

const float fallThreshold = 2.0; 
const float gyroThreshold = 5.0; 

Adafruit_MPU6050 mpu;

sensors_event_t accel, gyro, temp;

String message;

String phone_number = "+919391259921";        // Enter the recipient's phone number


float sensor_data[6];

void setup() {
                                              //Set the baud rate for the SerialSoftware objects  
                                              //    Device               Baud-rate
  mySerial1.begin(9600);                      //     Esp32           -       9600                  
  mySerial2.begin(115200);                    //     GSM             -       115200
  Serial.begin(115200);                       //     Seril-monitor   -       115200
                                              
  while(!mpu.begin())
     Serial.println("mpu6050 not connected"); //checking for mpu6050 connection

}

void loop() {


  mpu.getEvent(&accel, &gyro, &temp);         // getting mpu6050 values


  // loading mpu6050 values
  sensor_data[0] = accel.acceleration.x;     
  sensor_data[1] = accel.acceleration.y;
  sensor_data[2] = accel.acceleration.z; 
  sensor_data[3] = gyro.gyro.x;
  sensor_data[4] = gyro.gyro.y;
  sensor_data[5] = gyro.gyro.z;

  // caleculating resultant accelration and resultant gyro
  float totalAcceleration = sqrt(pow(accel.acceleration.x, 2) + pow(accel.acceleration.y, 2) + pow(accel.acceleration.z, 2));
  float totalGyro = sqrt(pow(gyro.gyro.x, 2) + pow(gyro.gyro.y, 2) + pow(gyro.gyro.z, 2));



  if (totalAcceleration > fallThreshold && totalGyro > gyroThreshold) {


    message = "Acceleration values: \n \
    X = " + String(sensor_data[0], 2) + " \n \
    Y = " + String(sensor_data[1], 2) + "  \n \
    Z = " + String(sensor_data[2], 2) + "\n \
    Gyro values: \n \
    X = " + String(sensor_data[3], 2) + " \n \
    Y = " + String(sensor_data[4], 2) + "  \n \
    Z = " + String(sensor_data[5], 2) + "\n \
    status: Fall Detected!!";


    
    if (mySerial1.isListening() || mySerial2.isListening()){
      delay(200);
      send_message(message);
      delay(200);
      send_call();

    }



  }

}


void senddata(){
   int i;
   mySerial1.print("<");
   for(i=0;i<sizeof(sensor_data)/sizeof(sensor_data[0]);i++){
      mySerial1.print(sensor_data[i]);
      mySerial1.print(",");
    
   }
   mySerial1.print(">");
}


void send_message(String message ){

  mySerial2.println("AT+CMGF=1");
  delay(200);                                        // Configuring TEXT mode
  mySerial2.println("AT+CMGS=\"" + phone_number + "\"");   
    delay(200);      
  mySerial2.print(message);
    delay(200);                                      //text content
  mySerial2.write(26);


}

void send_call(){
  delay(500);
  mySerial2.println("AT+CMGF=0");                                // Configuring TEXT mode
  delay(200);
  mySerial2.println("ATD" + phone_number + ";"); 

}


