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
}

void loop() {
  Dabble.processInput();             //this function is used to refresh data obtained from smartphone.Hence calling this function is mandatory in order to get data properly from your mobile.
  //print_Gyroscope_data();
  //print_Accelerometer_data();

  // float x_val = Sensor.getAccelerometerXaxis();
  // float y_val = Sensor.getAccelerometerYaxis();
  // float z_val = Sensor.getAccelerometerZaxis();  
  Serial.print("X-Axis: ");
  Serial.print(dead_zone(Sensor.getAccelerometerXaxis()));
  Serial.print("|");
  Serial.print("Y-Axis: ");
  Serial.println(dead_zone(Sensor.getAccelerometerYaxis()));
  
}
