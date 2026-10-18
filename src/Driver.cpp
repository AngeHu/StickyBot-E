#include <Arduino.h>
#include "Driver.h"



DCDriver::DCDriver(int ena, int in1, int in2, int enb, int in3, int in4)
    : ena_pin(ena), in1_pin(in1), in2_pin(in2),
        enb_pin(enb), in3_pin(in3), in4_pin(in4) {}

bool DCDriver::begin() {
    pinMode(ena_pin, OUTPUT);
    pinMode(in1_pin, OUTPUT);
    pinMode(in2_pin, OUTPUT);
    pinMode(enb_pin, OUTPUT);
    pinMode(in3_pin, OUTPUT);
    pinMode(in4_pin, OUTPUT);
    return true;
}
    
int DCDriver::to_speed(int speed) {
        return map(speed, 0, 100, 0, 255);
}

bool DCDriver::set_speed(int speed) {
    // Set speed for both motors
    analogWrite(ena_pin, to_speed(speed));
    analogWrite(enb_pin, to_speed(speed));
    return true;
}

bool DCDriver::set_direction(bool forward) {
    if (forward) {
        digitalWrite(in1_pin, HIGH);
        digitalWrite(in2_pin, LOW);
        digitalWrite(in3_pin, HIGH);
        digitalWrite(in4_pin, LOW);
    } else {
        digitalWrite(in1_pin, LOW);
        digitalWrite(in2_pin, HIGH);
        digitalWrite(in3_pin, LOW);
        digitalWrite(in4_pin, HIGH);
    }
    return true;
}

bool DCDriver::stop() {
    digitalWrite(in1_pin, LOW);
    digitalWrite(in2_pin, LOW);
    digitalWrite(in3_pin, LOW);
    digitalWrite(in4_pin, LOW);
    return true;
}

SMS_STS ServoDriver::st;

ServoDriver::ServoDriver(){}

bool ServoDriver::begin() {
    // Initialize the servo driver
    Serial.begin(1000000);
    st.pSerial = &Serial;
    delay(500);
    return true;
}


bool ServoDriver::move_sticker(int angle) {
    // Move the sticker servo to the specified angle
    
    int mapped_angle = to_angle(angle);
    st.WritePosEx(STICKER_SERVO, mapped_angle, 2500, 50);
    return true;
}

bool ServoDriver::tilt_head(int angle) {
    // Set the angle of the servo
    angle = constrain(angle, 0, 45);
    int mapped_angle = to_angle(angle);
    st.WritePosEx(PITCH_SERVO, mapped_angle, 2500, 50);
    return true;
}

bool ServoDriver::move_eyes(int angle) {
    // Move the eyes servo to the specified angle
    int mapped_angle = to_angle(angle);
    st.WritePosEx(ROLL_SERVO, mapped_angle, 2500, 50);
    return true;
}