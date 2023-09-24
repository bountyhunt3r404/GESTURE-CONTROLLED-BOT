/* ##############-IMPORTANT-##########*/
/* This code is only for PLATFORMIO!!!!, and will NOT work in ARDUINO IDE!!! 
   Use MAIN.INO in ARDUINO_CODE folder for ARDUINO IDE*/

#include <Arduino.h>

/*
    INSTRUCTIONS:
    !!!FOR FIRST TIME SETUP DOWNLOAD THE REQUIRED LIBRARIES FROM THE LIBARARY MANAGER:
       LIBRARIES USED:
       1. ESP32SERVO - link -> https://github.com/jkb-git/ESP32Servo
       2. DabbleESP32 - link -> https://github.com/STEMpedia/DabbleESP32
       3. SMOOTH - link -> https://github.com/ripred/Smooth
    
    !!!AS ESP32 IS A 3.3V DEVICE NECCASARY CAUTION SHOULD BE TAKEN WHILE CONNECTING SENSORS ANS OTHER DEVICES
*/

/*  #################################-PIN CONNECTIONS-#############################################
                                    ESP32          L293D
                                     D13    ->      I1
                                     D12    ->      I2
                                     D14    ->      I3
                                     D27    ->      I4
                                     D18    ->      ENA/EN1
                                     D19    ->      ENB/EN2

                                    ESP32       HALL SENSOR
                                     3.3V   ->      + (Vcc)
                                     GND    ->      - (GND)
                                     D21    ->      D0
                                     D32    ->      A0
                                    
                                    ESP32          SERVO
                                     D5     ->      PWM (Orange)
                                     Vin    ->      VCC (Red)
                                     GND    ->      GND (Brown)
*/

#define CUSTOM_SETTINGS
#define INCLUDE_SENSOR_MODULE
#include <ESP32Servo.h>
#include <DabbleESP32.h>
//#include <Smooth.h>



//MOTOR PIN CONNECTIONS
#define enableA 18
#define enableB 19

#define inputA1 13
#define inputA2 12

#define inputB1 14
#define inputB2 27

// HALL EFFECT SENSOR PINOUTS
#define hall_detect 21
#define hall_analog 32

//SERVO CONNECTIONS
Servo myservo;

//Limit of accelerometer values to neglet
float const_limit = 3.0;

//Setting speed of motors
int set_speed = 0;
const int min_motor_speed = 0;
const int max_motor_speed = 255;
const int precision = 100;


class Motor {
  bool pin1,pin2;
  int speed;
  public:
  Motor(int pin1,int pin2, int enable){
    pinMode(enable, OUTPUT);
    pinMode(pin1, OUTPUT);
    pinMode(pin2, OUTPUT);
  }

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

  void yAxisMotion(int a1,int a2,int enable){
      digitalWrite(a1, 1);
      digitalWrite(a2, 0);
      analogWrite(enable, set_speed);
  }
};

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
  //Serial.print(smooth_avg_filter(Sensor.getAccelerometerXaxis()));
  Serial.print(Sensor.getAccelerometerXaxis());
  Serial.print('|');
  Serial.print("AVG-Y: ");
  //Serial.print(smooth_avg_filter(Sensor.getAccelerometerYaxis()));
  Serial.print(Sensor.getAccelerometerYaxis());
  Serial.print("|");
  Serial.print("MAGNET: ");
  Serial.print(digitalRead(hall_detect));
  Serial.print("|");
  Serial.print("SPEED: ");
  Serial.println(set_speed);
  delay(1);
}

Motor L_motor(inputA1, inputA2, enableA);
Motor R_motor(inputB1, inputB2, enableB);


void setup() {
  Serial.begin(115200);    // Make sure your Serial Monitor is also set at this baud rate.
  Dabble.begin("MewYESP32"); // Set bluetooth name of your device!! (Make sure to set different name for each bot)

  pinMode(enableB, OUTPUT);
  pinMode(enableA, OUTPUT);
  pinMode(inputA1, OUTPUT);
  pinMode(inputA2, OUTPUT);
  pinMode(inputB1, OUTPUT);
  pinMode(inputB2, OUTPUT);
  myservo.attach(5); 

  bot_stop();
}

void loop() {
  Dabble.processInput();   //This function is used to refresh data obtained from smartphone.
                           //Hence calling this function is mandatory in order to get data properly from your mobile.

  //Stores the values recieved from the phone into their respective variable names
  float X = Sensor.getAccelerometerXaxis();
  float Y = Sensor.getAccelerometerYaxis();

  // //Stores the avg value returned from the fucntion
  // X = smooth_avg_filter(X);
  // Y = smooth_avg_filter(Y);

  //SERVO CONTROL SECTION
  if(digitalRead(hall_detect) == 1) {
    myservo.write(90);    //rotates the servo to 90 degree positon when a magnet is detected                         
    bot_stop();           //stops the bot when magnet is detected
    Serial.print("STOP");
  }

  else {
    myservo.write(0); //Bring servo to the neutral position
    
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
  }
  print_data();   //Prints the data such as X, Y of accelerometer and other necessary data.
}