#include <Arduino.h>
#include <SCServo.h>

// Change only this value when testing another servo ID.
const uint8_t TEST_SERVO_ID = 2;

const uint32_t SERVO_BAUD = 1000000;
const int POSITION_CENTER = 2048;
const int POSITION_LEFT = 1024;
const int POSITION_RIGHT = 3072;
const uint16_t MOVE_SPEED = 1000;
const uint8_t ACCELERATION = 50;
const unsigned long POSITION_DELAY_MS = 2000;

SMS_STS servo;

void moveTo(int position) {
    servo.WritePosEx(TEST_SERVO_ID, position, MOVE_SPEED, ACCELERATION);
    delay(POSITION_DELAY_MS);
}

void setup() {
    // The servo bus uses the Uno hardware UART on pins D0/D1.
    Serial.begin(SERVO_BAUD);
    servo.pSerial = &Serial;
    delay(500);

    // One servo only: center, left, right, then center.
    moveTo(POSITION_CENTER);
    moveTo(POSITION_LEFT);
    moveTo(POSITION_RIGHT);
    moveTo(POSITION_CENTER);
}

void loop() {
    // Keep the one-shot test from repeating.
    delay(1000);
}
