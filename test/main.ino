/* ##############-IMPORTANT-##########*/
/* This code is only for ARDUINO IDE!!! 
   Also install the required libraries
   link: https://github.com/STEMpedia/DabbleESP32 */


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


void bot_forward(int y) {
  analogWrite(enableA, y*23);
  Serial.print(y*23);
  analogWrite(enableB, y*23);
  digitalWrite(inputA1, 1);
  digitalWrite(inputA2, 0);
  digitalWrite(inputB1, 1);
  digitalWrite(inputB2, 0);
}

void bot_backward(int y) {
  analogWrite(enableB,-y*23);
  Serial.print(-y*23);
  analogWrite(enableA, -y*23);
  digitalWrite(inputA1, 0);
  digitalWrite(inputA2, 1);
  
  digitalWrite(inputB1, 0);
  digitalWrite(inputB2, 1);
}

void bot_left(int x) {
  analogWrite(enableB,-x*23);
  Serial.print(-x*23);
  analogWrite(enableA,-x*23);

  digitalWrite(inputA1, 1);
  digitalWrite(inputA2, 0);

  digitalWrite(inputB1, 0);
  digitalWrite(inputB2, 1);
}

void bot_right(int x) {
  analogWrite(enableB, -x*50);
  Serial.print(x*23);
  analogWrite(enableA, -x*23);

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
      bot_right(X);
      Serial.print("RIGHT\n");
    }

    else if (X < -const_limit) {
      bot_left(X);
      Serial.print("LEFT\n");
    }
  }

  //FORWARD-BACKWARD SECTION
  else if (dead_zone(Y)) {
    if (Y > const_limit) {
      bot_forward(Y);
      Serial.print("FORWARD\n");
    }

    else if (Y < -const_limit) {
      bot_backward(Y);
      Serial.print("BACKWARD\n");
    }   
  }
  else {

   }
  /*################################-END-##################################*/
}