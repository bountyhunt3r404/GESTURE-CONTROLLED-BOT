#include <Arduino.h>

/*
   With this block of phone sensor module you can access gyroscope values of your smartphone.
   Gyroscope gives you angular acceleration in x,y and z axis.

   You can reduce the size of library compiled by enabling only those modules that you want
   to   use. For this first define CUSTOM_SETTINGS followed by defining
   INCLUDE_modulename.

   Explore more on: https://thestempedia.com/docs/dabble/phone-sensors-module/
*/

/*  ##################################-PIN CONNECTIONS-#############################################
                                    ESP32         L293D
                                     D13    ->     I1
                                     D12    ->     I2
                                     D14    ->     I3
                                     D27    ->     I4
*/

#define CUSTOM_SETTINGS
#define INCLUDE_SENSOR_MODULE

#include <DabbleESP32.h>

//MOTOR PIN CONNECTIONS
#define enableA 34
#define enableB 35

#define inputA1 13
#define inputA2 12

#define inputB1 14
#define inputB2 27

//Limit of 
float const_limit = 3.0;

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
bool dead_zone(float val, float limit = const_limit) {
  if (val >= -limit && val <= limit) {
    return 0;
  }

  else {
    return 1;
  }
}


void bot_forward() {
  digitalWrite(inputA1, 1);
  digitalWrite(inputA2, 0);

  digitalWrite(inputB1, 1);
  digitalWrite(inputB2, 0);
}

void bot_backward() {
  digitalWrite(inputA1, 0);
  digitalWrite(inputA2, 1);

  digitalWrite(inputB1, 0);
  digitalWrite(inputB2, 1);
}

void bot_left() {
  digitalWrite(inputA1, 1);
  digitalWrite(inputA2, 0);

  digitalWrite(inputB1, 0);
  digitalWrite(inputB2, 1);
}

void bot_right() {
  digitalWrite(inputA1, 0);
  digitalWrite(inputA2, 1);

  digitalWrite(inputB1, 1);
  digitalWrite(inputB2, 0);
}



void setup() {
  Serial.begin(115200);    // Make sure your Serial Monitor is also set at this baud rate.
  Dabble.begin("MYESP32"); // Set bluetooth name of your device!! (Make sure to set different name for each bot)

  pinMode(enableB, OUTPUT);
  pinMode(enableA, OUTPUT);
  pinMode(inputA1, OUTPUT);
  pinMode(inputA2, OUTPUT);
  pinMode(inputB1, OUTPUT);
  pinMode(inputB2, OUTPUT);
  
}

void loop() {
  Dabble.processInput();   //this function is used to refresh data obtained from smartphone.Hence calling this function is mandatory in order to get data properly from your mobile.

  //print_Accelerometer_data();

  //Stores the values recieved from the phone into their respective variable names
  float X = Sensor.getAccelerometerXaxis();
  float Y = Sensor.getAccelerometerYaxis();

  /*#####################-MOTOR CONTROL SECTION-#########################*/

  //LEFT-RIGHT SECTION
  if (dead_zone(X)) {
    if (X > const_limit) {
      bot_right();
      Serial.print("RIGHT");
    }

    else if (X < -const_limit) {
      bot_left();
      Serial.print("LEFT");
    }
  }

  //FORWARD-BACKWARD SECTION
  else if (dead_zone(Y)) {
    if (Y > const_limit) {
      bot_forward();
      Serial.print("FORWARD");
    }

    else if (Y < -const_limit) {
      bot_backward();
      Serial.print("BACKWARD");
    }   
  }

  /*################################-END-##################################*/
}
