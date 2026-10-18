#include <Arduino.h>

const uint8_t FL_EN = 9;
const uint8_t FL_IN1 = 8;
const uint8_t FL_IN2 = 10;
const uint8_t FR_EN = 11;
const uint8_t FR_IN1 = 12;
const uint8_t FR_IN2 = 13;
const uint8_t RL_EN = 3;
const uint8_t RL_IN1 = 2;
const uint8_t RL_IN2 = 4;
const uint8_t RR_EN = 5;
const uint8_t RR_IN1 = 6;
const uint8_t RR_IN2 = 7;

const uint8_t SPEED = 20;
const unsigned long DIRECTION_TIME_MS = 800;

void drive(bool backward) {
    // Disable PWM and set both inputs low before changing direction.
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

    delay(100);

    digitalWrite(FL_IN1, backward ? HIGH : LOW);
    digitalWrite(FL_IN2, backward ? LOW : HIGH);
    digitalWrite(FR_IN1, backward ? HIGH : LOW);
    digitalWrite(FR_IN2, backward ? LOW : HIGH);
    digitalWrite(RL_IN1, backward ? HIGH : LOW);
    digitalWrite(RL_IN2, backward ? LOW : HIGH);
    digitalWrite(RR_IN1, backward ? HIGH : LOW);
    digitalWrite(RR_IN2, backward ? LOW : HIGH);

    uint8_t pwm = SPEED * 255 / 100;
    analogWrite(FL_EN, pwm);
    analogWrite(FR_EN, pwm);
    analogWrite(RL_EN, pwm);
    analogWrite(RR_EN, pwm);
}

void setup() {
    const uint8_t pins[] = {
        FL_EN, FL_IN1, FL_IN2,
        FR_EN, FR_IN1, FR_IN2,
        RL_EN, RL_IN1, RL_IN2,
        RR_EN, RR_IN1, RR_IN2
    };

    for (uint8_t i = 0; i < sizeof(pins); ++i) {
        pinMode(pins[i], OUTPUT);
    }

}

void loop() {
    drive(true);
    delay(DIRECTION_TIME_MS);

    drive(false);
    delay(DIRECTION_TIME_MS);
}
