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

//Left Motor
int IN1_A = 34;
int IN2_A = 35;

//Right Motor
int IN1_B = 32;
int IN2_B = 33;

int enableA = 34;
int enableB = 35;

int inputA1 = 13;
int inputA2 = 12;
int inputB1 = 14;
int inputB2 = 27;

void print_Accelerometer_data()
{
  Serial.print("X_axis: ");
  Serial.print(Sensor.getAccelerometerXaxis(), 4);
  Serial.print('\t');
  Serial.print("Y_axis: ");
  Serial.print(Sensor.getAccelerometerYaxis(), 4);
  Serial.print('\t');
  Serial.print("Z_axis: ");
  Serial.print(Sensor.getAccelerometerZaxis(), 4);
  Serial.println();
  delay(10);
}

bool dead_zone(float val) {
  if (val >= -3.000 && val <=3.000) {
    return 0;
  }

  else {
    return 1;
  }
}

void setup() {
  Serial.begin(115200);   // make sure your Serial Monitor is also set at this baud rate.
  Dabble.begin("MyEsp32"); //set bluetooth name of your device

  pinMode(IN1_A, OUTPUT);
  pinMode(IN1_B, OUTPUT);
  pinMode(IN2_A, OUTPUT);
  pinMode(IN2_B, OUTPUT);
  pinMode(enableB, OUTPUT);
  pinMode(enableA, OUTPUT);
  pinMode(inputA1, OUTPUT);
  pinMode(inputA2, OUTPUT);
  pinMode(inputB1, OUTPUT);
  pinMode(inputB2, OUTPUT);
  
}

void loop() {
  Dabble.processInput();             //this function is used to refresh data obtained from smartphone.Hence calling this function is mandatory in order to get data properly from your mobile.
  //print_Gyroscope_data();
  print_Accelerometer_data();

  // float x_val = Sensor.getAccelerometerXaxis();
  // float y_val = Sensor.getAccelerometerYaxis();
  // float z_val = Sensor.getAccelerometerZaxis();  
  // Serial.print("X-Axis: ");
  // Serial.print(dead_zone(Sensor.getAccelerometerXaxis()));
  // Serial.print("|");
  // Serial.print("Y-Axis: ");
  // Serial.println(dead_zone(Sensor.getAccelerometerYaxis()));

  float x = Sensor.getAccelerometerXaxis();
  float y = Sensor.getAccelerometerYaxis();
  //float z = Sensor.getAccelerometerZaxis();


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
