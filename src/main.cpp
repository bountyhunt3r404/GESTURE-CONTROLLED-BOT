#include <Arduino.h>

/*
   With this block of phone sensor module you can access gyroscope values of your smartphone.
   Gyroscope gives you angular acceleration in x,y and z axis.

   You can reduce the size of library compiled by enabling only those modules that you want
   to   use. For this first define CUSTOM_SETTINGS followed by defining
   INCLUDE_modulename.

   Explore more on: https://thestempedia.com/docs/dabble/phone-sensors-module/
*/

#define CUSTOM_SETTINGS
#define INCLUDE_SENSOR_MODULE

#include <DabbleESP32.h>

//MOTOR PIN CONNECTIONS
int enableA = 34;
int enableB = 35;

int inputA1 = 13;
int inputA2 = 12;

int inputB1 = 14;
int inputB2 = 27;

//Limit of 
float limit = 3.0;

/*This Function is used to print the Sensor values 
  recieved from Phone's Accelerometer*/ 
void print_Accelerometer_data() {
  Serial.print("X-Axis: ");
  Serial.print(Sensor.getAccelerometerXaxis(), 4);
  Serial.print('|');
  Serial.print("Y-Axis: ");
  Serial.print(Sensor.getAccelerometerYaxis(), 4);
  Serial.print('|');
  Serial.print("Z-Axis: ");
  Serial.println(Sensor.getAccelerometerZaxis(), 4);
  delay(10);
}

/*The function */
bool dead_zone(float val, float limit) {
  if (val >= -limit && val <= limit) {
    return 0;
  }

  else {
    return 1;
  }
}

void setup() {
  Serial.begin(115200);   // make sure your Serial Monitor is also set at this baud rate.
  Dabble.begin("MYESP32"); //set bluetooth name of your device

  pinMode(enableB, OUTPUT);
  pinMode(enableA, OUTPUT);
  pinMode(inputA1, OUTPUT);
  pinMode(inputA2, OUTPUT);
  pinMode(inputB1, OUTPUT);
  pinMode(inputB2, OUTPUT);
  
}

void loop() {
  Dabble.processInput();      //this function is used to refresh data obtained from smartphone.Hence calling this function is mandatory in order to get data properly from your mobile.

  print_Accelerometer_data();

  float x = Sensor.getAccelerometerXaxis();
  float y = Sensor.getAccelerometerYaxis();



  if (x>-3 && x<3 && y<3 && y>-3){
    digitalWrite(inputA1,LOW);
    digitalWrite(inputA2,LOW);
    digitalWrite(inputB1,LOW);
    digitalWrite(inputB2,LOW);
    Serial.print("Motors off");
  }


  if (y>3 || y<-3){
    if (y<-3){
      digitalWrite(inputA1,HIGH);
      digitalWrite(inputB1,HIGH);
      Serial.println("fwd");
    }
    else if(y>3){
      digitalWrite(inputA2,HIGH);
      digitalWrite(inputB2,HIGH);
      Serial.println("bkwd");
    }
  }

  if (x>3 || x<-3){
    if (x<-3){                        //right
      digitalWrite(inputA2,HIGH);
      digitalWrite(inputB1,HIGH);
      Serial.println("right");
    }
    else if(x>3){                     //left
      digitalWrite(inputA1,HIGH);
      digitalWrite(inputB2,HIGH);
      Serial.println("left");
    }
  }
}
