#include <Arduino.h>
#include <Wire.h>

// Keep the motor driver disabled while this diagnostic sketch is running.
const uint8_t MOTOR_PINS[] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};
const uint8_t MOTOR_PIN_COUNT = sizeof(MOTOR_PINS) / sizeof(MOTOR_PINS[0]);

uint8_t imu_address = 0;
uint8_t last_i2c_error = 0;

void disableMotorPins() {
    for (uint8_t i = 0; i < MOTOR_PIN_COUNT; ++i) {
        pinMode(MOTOR_PINS[i], OUTPUT);
        digitalWrite(MOTOR_PINS[i], LOW);
    }
}

bool writeRegister(uint8_t reg, uint8_t value) {
    Wire.beginTransmission(imu_address);
    Wire.write(reg);
    Wire.write(value);
    last_i2c_error = Wire.endTransmission(true);
    return last_i2c_error == 0;
}

bool readAcceleration(float &x, float &y, float &z) {
    Wire.beginTransmission(imu_address);
    Wire.write(0x3B);
    if (Wire.endTransmission(false) != 0 || Wire.requestFrom(imu_address, (uint8_t)6) != 6) {
        return false;
    }

    int16_t raw_x = (Wire.read() << 8) | Wire.read();
    int16_t raw_y = (Wire.read() << 8) | Wire.read();
    int16_t raw_z = (Wire.read() << 8) | Wire.read();

    x = raw_x / 16384.0f;
    y = raw_y / 16384.0f;
    z = raw_z / 16384.0f;
    return true;
}

bool findImu() {
    imu_address = 0x68;

    // Match the known-good startup sequence exactly.
    Wire.beginTransmission(imu_address);
    Wire.write(0x6B);
    Wire.write(0x00);
    last_i2c_error = Wire.endTransmission(true);
    return last_i2c_error == 0;
}

void setup() {
    disableMotorPins();
    Serial.begin(19200);
    Wire.begin();

    Serial.println("MPU6050 sensor-only test");

    if (!findImu()) {
        Serial.print("I2C error code: ");
        Serial.println(last_i2c_error);
        Serial.println("MPU6050 did not acknowledge address 0x68");
        return;
    }

    Serial.print("MPU6050 found at 0x");
    Serial.println(imu_address, HEX);

    writeRegister(0x1A, 0x03); // Low-pass filter
    writeRegister(0x1C, 0x00); // Accelerometer: +/-2 g
    Serial.println("Reading acceleration...");
}

void loop() {
    static unsigned long last_read = 0;
    if (millis() - last_read < 500) {
        return;
    }
    last_read = millis();

    if (!imu_address) {
        return;
    }

    float x;
    float y;
    float z;
    if (!readAcceleration(x, y, z)) {
        Serial.println("Acceleration read failed");
        return;
    }

    Serial.print("accel g: x=");
    Serial.print(x, 3);
    Serial.print(" y=");
    Serial.print(y, 3);
    Serial.print(" z=");
    Serial.println(z, 3);
}
