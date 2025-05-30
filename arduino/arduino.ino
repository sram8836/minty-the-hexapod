#include <Arduino_LSM6DS3.h>

float ax, ay, az;    // Accelerometer data
float gx, gy, gz;    // Gyroscope data (degrees/sec)
float roll = 0, pitch = 0;

unsigned long lastTime;
float alpha = 0.98;  // Filter coefficient

void setup() {
  Serial.begin(9600);
  while (!Serial);

  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }
  Serial.println("IMU initialized.");

  lastTime = millis();
}

void loop() {
  if (IMU.accelerationAvailable() && IMU.gyroscopeAvailable()) {
    IMU.readAcceleration(ax, ay, az);
    IMU.readGyroscope(gx, gy, gz);

    unsigned long now = millis();
    float dt = (now - lastTime) / 1000.0;
    lastTime = now;

    // Estimate roll and pitch from accelerometer (in degrees)
    roll = atan2(ay, az) * RAD_TO_DEG;
    pitch = atan2(-ax, sqrt(ay * ay + az * az)) * RAD_TO_DEG;
    
//    // Estimate roll and pitch from accelerometer (in degrees)
//    float accelRoll = atan2(ay, az) * RAD_TO_DEG;
//    float accelPitch = atan2(-ax, sqrt(ay * ay + az * az)) * RAD_TO_DEG;
//
//    // Complementary filter to combine gyro and accel
//    roll = alpha * (roll + gx * dt) + (1 - alpha) * accelRoll;
//    pitch = alpha * (pitch + gy * dt) + (1 - alpha) * accelPitch;

    // Output
    Serial.print("Roll: ");
    Serial.print(roll, 2);
    Serial.print(" | Pitch: ");
    Serial.println(pitch, 2);
  }

  delay(500);
}
