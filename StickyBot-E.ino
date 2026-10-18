#include <Arduino.h>
#include <SCServo.h>

// Front board
const uint8_t FL_EN = 9;
const uint8_t FL_IN1 = 8;
const uint8_t FL_IN2 = 10;
const uint8_t FR_EN = 11;
const uint8_t FR_IN1 = 12;
const uint8_t FR_IN2 = 13;

// Rear board
const uint8_t RL_EN = 3;
const uint8_t RL_IN1 = 2;
const uint8_t RL_IN2 = 4;
const uint8_t RR_EN = 5;
const uint8_t RR_IN1 = 6;
const uint8_t RR_IN2 = 7;

const uint8_t STICKER_ID = 1;
const int16_t STICKER_SPEED = -3400;
const uint8_t STICKER_ACCELERATION = 50;

const uint8_t FORWARD_SPEED = 20;
const uint8_t DIAGONAL_SPEED = 35;
const uint8_t BACKWARD_SPEED = 20;
const unsigned long START_DELAY_MS = 1000;
const unsigned long FORWARD_DURATION_MS = 1000;
const unsigned long DIAGONAL_DURATION_MS = 4000;
const unsigned long BACKWARD_DURATION_MS = 500;
const unsigned long DIRECTION_PAUSE_MS = 100;

SMS_STS sticker;

void motor(uint8_t en, uint8_t in1, uint8_t in2, int command) {
    command = constrain(command, -100, 100);

    if (command == 0) {
        analogWrite(en, 0);
        digitalWrite(in1, LOW);
        digitalWrite(in2, LOW);
        return;
    }

    bool forward = command > 0;
    digitalWrite(in1, forward ? LOW : HIGH);
    digitalWrite(in2, forward ? HIGH : LOW);
    analogWrite(en, abs(command) * 255 / 100);
}

void drive(int fl, int fr, int rl, int rr) {
    // Disable the bridge and set a neutral state before changing direction.
    analogWrite(FL_EN, 0);
    analogWrite(FR_EN, 0);
    analogWrite(RL_EN, 0);
    analogWrite(RR_EN, 0);

    digitalWrite(FL_IN1, LOW);
    digitalWrite(FL_IN2, LOW);
    digitalWrite(FR_IN1, LOW);
    digitalWrite(FR_IN2, LOW);
    digitalWrite(RL_IN1, LOW);
    digitalWrite(RL_IN2, LOW);
    digitalWrite(RR_IN1, LOW);
    digitalWrite(RR_IN2, LOW);

    delay(DIRECTION_PAUSE_MS);

    motor(FL_EN, FL_IN1, FL_IN2, fl);
    motor(FR_EN, FR_IN1, FR_IN2, fr);
    motor(RL_EN, RL_IN1, RL_IN2, rl);
    motor(RR_EN, RR_IN1, RR_IN2, rr);
}

void stopMotors() {
    drive(0, 0, 0, 0);
}

void forward(uint8_t speed) {
    drive(speed, speed, speed, speed);
}

void diagonalLeft(uint8_t speed) {
    int small = speed * 3 / 5;
    drive(-small, speed, speed, -small);
}

void backward(uint8_t speed) {
    drive(-speed, -speed, -speed, -speed);
}

void setupMotors() {
    const uint8_t pins[] = {
        FL_EN, FL_IN1, FL_IN2,
        FR_EN, FR_IN1, FR_IN2,
        RL_EN, RL_IN1, RL_IN2,
        RR_EN, RR_IN1, RR_IN2
    };

    for (uint8_t i = 0; i < sizeof(pins); ++i) {
        pinMode(pins[i], OUTPUT);
    }

    stopMotors();
}

void setupServo() {
    Serial.begin(1000000);
    sticker.pSerial = &Serial;
    sticker.IOTimeOut = 10;
    delay(500);
}

void setStickerSpeed(int16_t speed) {
    sticker.WriteSpe(STICKER_ID, speed, STICKER_ACCELERATION);
}

void setup() {
    setupMotors();
    setupServo();
}

void loop() {
    stopMotors();
    delay(START_DELAY_MS);

    forward(FORWARD_SPEED);
    delay(FORWARD_DURATION_MS);

    stopMotors();
    delay(DIRECTION_PAUSE_MS);

    sticker.EnableTorque(STICKER_ID, 1);
    sticker.WheelMode(STICKER_ID);
    setStickerSpeed(STICKER_SPEED);
    diagonalLeft(DIAGONAL_SPEED);
    delay(DIAGONAL_DURATION_MS);

    setStickerSpeed(0);
    sticker.EnableTorque(STICKER_ID, 0);
    stopMotors();
    delay(DIRECTION_PAUSE_MS);

    backward(BACKWARD_SPEED);
    delay(BACKWARD_DURATION_MS);

    stopMotors();
    while (true) {
        delay(1000);
    }
}
