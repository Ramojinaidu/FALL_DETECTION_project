/*
/////////////////////////////////////////////////////////////////////////////////////////
***************************COMMUNITY SERVICE PROJECT********************************
    
      Project_name      : Fall-detection Using arduino
      Project_guide     : 
      Project_members   :     Name                                       Roll-Number
                          1) M.Harshith                 -                23021A0429
                          2) G.Ravi Kumar               -                23021A0417
                          3) P.Sai Saran                -                23021A0436
                          4) M.Pavani                   -                23021A0432
                          5) Varshini                   -                24021A0459

      Branch            : Electronics And Communication Engineering
      Course            : B-Tech
      Year              : 2nd year

/////////////////////////////////////////////////////////////////////////////////////////
*/



#include <SoftwareSerial.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>


SoftwareSerial mySerial2(3,2);              // Set up a new SoftwareSerial object with RX in digital pin 03 and TX in digital pin 02 for GSM module

const float fallThreshold = 2.0;            // Threashold values fall recognization
const float gyroThreshold = 5.0; 

Adafruit_MPU6050 mpu;
sensors_event_t accel, gyro, temp;

String message;
String phone_number = "+ZZxxxxxxxxxx";        // Enter the recipient's phone number


float sensor_data[6];

void setup() {
                                              //Set the baud rate for the SerialSoftware objects  
                                              //    Device               Baud-rate
  mySerial2.begin(115200);                    //     GSM             -       115200
                                              
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


//Condition for checking fall and sending Message and Call
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


    
    if (mySerial2.isListening()){
      delay(200);
      send_message(message);
      delay(200);
      send_call();
    }



  }

}



//send message
void send_message(String message ){

  mySerial2.println("AT+CMGF=1");                               // Configuring TEXT mode
  delay(200);                                                     
  mySerial2.println("AT+CMGS=\"" + phone_number + "\"");        // Configuring phone number
  delay(200);      
  mySerial2.print(message);                                     //sending text content
  delay(200);                                                  
  mySerial2.write(26);


}


// send call
void send_call(){
  delay(500);
  mySerial2.println("AT+CMGF=0");                                 // Configuring CALL mode
  delay(200);
  mySerial2.println("ATD" + phone_number + ";");                  //Sending Call
}


