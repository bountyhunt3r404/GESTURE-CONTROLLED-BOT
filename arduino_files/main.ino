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

/*  #################################-PIN CONNECTIONS-#############################################
                                    ESP32         L293D
                                     D13    ->     I1
                                     D12    ->     I2
                                     D14    ->     I3
                                     D27    ->     I4
                                     D25    ->     EN1
                                     D26    ->     EN2

*/

#define CUSTOM_SETTINGS
#define INCLUDE_SENSOR_MODULE

#include <DabbleESP32.h>
#include <Smooth.h>



//MOTOR PIN CONNECTIONS
#define enableA 26
#define enableB 25

#define inputA1 13
#define inputA2 12

#define inputB1 14
#define inputB2 27

//Limit of accelerometer values to neglet
float const_limit = 3.0;

//Setting speed of motors
int set_speed = 110;
const int min_motor_speed = 65;
const int max_motor_speed = 255;
const int precision = 100;


/*The function takes RAW value of ACCELEROMETER and returns 1 or 0
  if the value crosses the const_limit*/
bool dead_zone(float val, float limit = const_limit) {
  if (val >= -limit && val <= limit) {
    return 0;
  }

  else {
    return 1;
  }
}

/*This function uses smooth library and takes int and float as INPUT
  and returns the AVGERAGE VALUE of the same data type.*/
float smooth_data(float val) {
  Smooth avg(10);

  if(val < 0) {
    val *= -1;
    avg.add(val);
    
    return -avg();
  }

  else {
    avg.add(val);

    return avg();
  }
}

/*###################-BOT MOVEMENT FUNCTIONS-########################*/

void bot_forward() {
  digitalWrite(inputA1, 1);
  digitalWrite(inputA2, 0);
  analogWrite(enableB, set_speed);

  digitalWrite(inputB1, 1);
  digitalWrite(inputB2, 0);
  analogWrite(enableB, set_speed);
}

void bot_backward() {
  digitalWrite(inputA1, 0);
  digitalWrite(inputA2, 1);
  analogWrite(enableB, set_speed);

  digitalWrite(inputB1, 0);
  digitalWrite(inputB2, 1);
  analogWrite(enableB, set_speed);

}

void bot_left() {
  digitalWrite(inputA1, 1);
  digitalWrite(inputA2, 0);
  analogWrite(enableB, set_speed);

  digitalWrite(inputB1, 0);
  digitalWrite(inputB2, 1);
  analogWrite(enableB, set_speed);
}

void bot_right() {
  digitalWrite(inputA1, 0);
  digitalWrite(inputA2, 1);
  analogWrite(enableB, set_speed);

  digitalWrite(inputB1, 1);
  digitalWrite(inputB2, 0);
  analogWrite(enableB, set_speed);
}

void bot_stop() {
  digitalWrite(inputA1, 0);
  digitalWrite(inputA2, 0);
  analogWrite(enableA, 0);

  digitalWrite(inputB1, 0);
  digitalWrite(inputB2, 0);
  analogWrite(enableB, 0);
}

/*#############################-END-##################################*/

/*This Function is used to print the Sensor values 
  recieved from Phone's Accelerometer and other parameters
  such as SMOOTHED VALLUES and SPEED  
*/ 
void print_data() {
  Serial.print("-|");
  Serial.print("X-Axis: ");
  Serial.print(Sensor.getAccelerometerXaxis(), 4);
  Serial.print('|');
  Serial.print("Y-Axis: ");
  Serial.print(Sensor.getAccelerometerYaxis(), 4);
  Serial.print('|');
  Serial.print("AVG-X: ");
  Serial.print(smooth_data(Sensor.getAccelerometerXaxis()));
  Serial.print('|');
  Serial.print("AVG-Y: ");
  Serial.print(smooth_data(Sensor.getAccelerometerYaxis()));
  Serial.print("|");
  Serial.print("SPEED: ");
  Serial.println(set_speed);
  delay(1);
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
  Dabble.processInput();   //This function is used to refresh data obtained from smartphone.
                           //Hence calling this function is mandatory in order to get data properly from your mobile.

  //Stores the values recieved from the phone into their respective variable names
  float X = Sensor.getAccelerometerXaxis();
  float Y = Sensor.getAccelerometerYaxis();

  X = smooth_data(X);
  Y = smooth_data(Y);

  /*#####################-MOTOR CONTROL SECTION-#########################*/

  //Speed Control of motors

  if (dead_zone(X)) {
    if (X>0) {
      set_speed = map(X*precision, 3*precision, 11*precision, min_motor_speed, max_motor_speed);
    }

    else {
      set_speed = map(-X*precision, 3*precision, 11*precision, min_motor_speed, max_motor_speed);
    }
  }

  else if (dead_zone(Y)) {
    if (Y>0) {
      set_speed = map(Y*precision, 3*precision, 11*precision, min_motor_speed, max_motor_speed);
    }
    
    else {
      set_speed = map(-Y*precision, 3*precision, 11*precision, min_motor_speed, max_motor_speed);
    }
  }

  else {
    set_speed = 0;
  }

  //LEFT-RIGHT SECTION
  if (dead_zone(X)) {
    if (X < -const_limit) {
      bot_right();
      Serial.print("RIGHT");
    }

    else if (X > const_limit) {
      bot_left();
      Serial.print("LEFT");
    }
  }

  //FORWARD-BACKWARD SECTION
  else if (dead_zone(Y)) {
    if (Y < -const_limit) {
      bot_forward();
      Serial.print("FORWARD");
    }

    else if (Y > const_limit) {
      bot_backward();
      Serial.print("BACKWARD");
    }   
  }

  else {
    bot_stop();
    Serial.print("STOP");
  }

  /*################################-END-##################################*/

  //Prints the data such as X, Y of accelerometer and other necessary data.
  print_data();
}