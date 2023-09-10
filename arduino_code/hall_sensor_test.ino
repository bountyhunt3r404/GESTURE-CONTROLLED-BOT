//This program is used to test the interface between ESP8266 AND HALL EFFCT SENSOR

/*         PINOUTS
      SENDOR     ESP8266
        +    ->    3.3V
        -    ->    GND
        A0   ->    A0
        D0   ->    D5
*/

#define detect_pin D5
#define analog_read_pin A0


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(detect_pin, INPUT);
  pinMode(analog_read_pin, INPUT);
  Serial.println("Ready...");
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.print(analogRead(analog_read_pin));
  Serial.print("\t");
  Serial.println(digitalRead(detect_pin));
  delay(100);
}
