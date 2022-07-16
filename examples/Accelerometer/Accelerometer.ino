#include <Wire.h>
#include <Accelerometer.h>

using namespace IntroSatLib;
Accelerometer accel(Wire);

int16_t ax = 0, ay = 0, az = 0;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  accel.Init();
  delay(1000);
  Serial.println("ax, ay, az");
}

void loop() {

  Serial.print(accel.X());Serial.print(" ");
  Serial.print(accel.Y());Serial.print(" ");
  Serial.println(accel.Z());
  delay(100);
}
