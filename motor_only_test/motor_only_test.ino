#include <Arduino.h>

enum Wheel {
    FRONT_LEFT = 0,
    FRONT_RIGHT,
    REAR_LEFT,
    REAR_RIGHT,
    WHEEL_COUNT
};

struct WheelConfig {
    uint8_t enable_pin;
    uint8_t in1_pin;
    uint8_t in2_pin;
};

// Front board: ENA=9, IN1=8, IN2=10, IN3=12, IN4=13, ENB=11
// Rear board:  ENA=3, IN1=2, IN2=4,  IN3=6,  IN4=7,  ENB=5
const WheelConfig WHEELS[WHEEL_COUNT] = {
    {9,  8,  10}, // front-left
    {11, 12, 13}, // front-right
    {3,  2,  4},  // rear-left
    {5,  6,  7}   // rear-right
};

const int FORWARD_SPEED = 20;
const int DIAGONAL_SPEED = 35;
const int BACKWARD_SPEED = 35;
const unsigned long FORWARD_DURATION_MS = 1000;
const unsigned long DIAGONAL_DURATION_MS = 2000;
const unsigned long BACKWARD_DURATION_MS = 1000;
const unsigned long TRANSITION_PAUSE_MS = 1000;

class MotorDrive {
public:
    void begin() {
        for (uint8_t wheel = 0; wheel < WHEEL_COUNT; ++wheel) {
            pinMode(WHEELS[wheel].enable_pin, OUTPUT);
            pinMode(WHEELS[wheel].in1_pin, OUTPUT);
            pinMode(WHEELS[wheel].in2_pin, OUTPUT);

            analogWrite(WHEELS[wheel].enable_pin, 0);
            digitalWrite(WHEELS[wheel].in1_pin, LOW);
            digitalWrite(WHEELS[wheel].in2_pin, LOW);
        }
    }

    void forward(int speed) {
        driveMix(speed, speed, speed, speed);
    }

    void diagonalForwardLeft(int speed) {
        // Left-dominant motion with a small forward component.
        driveMix(-speed * 3 / 5, speed, speed, -speed * 3 / 5);
    }

    void backward(int speed) {
        driveMix(-speed, -speed, -speed, -speed);
    }

    void stop() {
        for (uint8_t wheel = 0; wheel < WHEEL_COUNT; ++wheel) {
            analogWrite(WHEELS[wheel].enable_pin, 0);
            digitalWrite(WHEELS[wheel].in1_pin, LOW);
            digitalWrite(WHEELS[wheel].in2_pin, LOW);
        }
    }

private:
    void driveMix(int front_left, int front_right, int rear_left, int rear_right) {
        const int commands[WHEEL_COUNT] = {
            front_left,
            front_right,
            rear_left,
            rear_right
        };

        // Disable PWM before changing direction signals.
        for (uint8_t wheel = 0; wheel < WHEEL_COUNT; ++wheel) {
            analogWrite(WHEELS[wheel].enable_pin, 0);
        }

        for (uint8_t wheel = 0; wheel < WHEEL_COUNT; ++wheel) {
            applyWheelCommand(static_cast<Wheel>(wheel), commands[wheel]);
        }
    }

    void applyWheelCommand(Wheel wheel, int command) {
        const WheelConfig &config = WHEELS[wheel];
        int bounded_command = constrain(command, -100, 100);
        int magnitude = abs(bounded_command);

        if (magnitude == 0) {
            digitalWrite(config.in1_pin, LOW);
            digitalWrite(config.in2_pin, LOW);
            analogWrite(config.enable_pin, 0);
            return;
        }

        bool forward = bounded_command > 0;
        digitalWrite(config.in1_pin, forward ? LOW : HIGH);
        digitalWrite(config.in2_pin, forward ? HIGH : LOW);
        analogWrite(config.enable_pin, map(magnitude, 0, 100, 0, 255));
    }
};

MotorDrive drive;

void setup() {
    drive.begin();
    drive.stop();
}

void loop() {
    drive.stop();
    delay(TRANSITION_PAUSE_MS);

    drive.forward(FORWARD_SPEED);
    delay(FORWARD_DURATION_MS);

    drive.stop();
    delay(TRANSITION_PAUSE_MS);

    drive.diagonalForwardLeft(DIAGONAL_SPEED);
    delay(DIAGONAL_DURATION_MS);

    drive.stop();
    delay(TRANSITION_PAUSE_MS);

    drive.backward(BACKWARD_SPEED);
    delay(BACKWARD_DURATION_MS);

    drive.stop();
    while (true) {
        delay(1000);
    }
}
