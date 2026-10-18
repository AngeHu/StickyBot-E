#include <Arduino.h>
#include <SCServo.h>

// This is a standalone motor-test sketch. It intentionally does not modify
// StickyBot-E.ino or the existing Driver class.

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

    // Change these values when a wheel's physical forward direction is
    // opposite to the current GPIO convention.
    uint8_t forward_in1_level;
    uint8_t forward_in2_level;
};

// During reset and the bootloader, Arduino GPIO pins are high-impedance.
// The motor driver's enable inputs should therefore also have hardware
// pull-down resistors so the motors remain disabled before setup() starts.

// Board wiring:
//   Rear board  ("Front board"): ENA=3, IN1=2, IN2=4,
//                                IN3=6, IN4=7, ENB=5
//   Front board ("Back board"):  ENA=9, IN1=8, IN2=10,
//                                IN3=12, IN4=13, ENB=11
//
// Channel mapping assumes A = left wheel and B = right wheel:
//   front A = front-left, front B = front-right
//   rear A  = rear-left,  rear B  = rear-right
const WheelConfig WHEELS[WHEEL_COUNT] = {
    // enable, IN1, IN2, forward IN1, forward IN2
    {9,  8,  10, LOW, HIGH}, // FRONT_LEFT  (front board ENA)
    {11, 12, 13, LOW, HIGH}, // FRONT_RIGHT (front board ENB)
    {3,  2,  4,   LOW, HIGH}, // REAR_LEFT   (rear board ENA)
    {5,  6,  7,   LOW, HIGH}  // REAR_RIGHT  (rear board ENB)
};

class MecanumDrive {
public:
    void begin() {
        // Match the old startup behavior: configure every motor pin before
        // applying any movement command, while keeping every enable disabled.
        for (uint8_t wheel = 0; wheel < WHEEL_COUNT; ++wheel) {
            pinMode(WHEELS[wheel].enable_pin, OUTPUT);
            pinMode(WHEELS[wheel].in1_pin, OUTPUT);
            pinMode(WHEELS[wheel].in2_pin, OUTPUT);

            analogWrite(WHEELS[wheel].enable_pin, 0);
            digitalWrite(WHEELS[wheel].in1_pin, LOW);
            digitalWrite(WHEELS[wheel].in2_pin, LOW);
        }

        // Preload a valid direction on every channel before any PWM is
        // enabled, like the old set_direction(true) call.
        preloadForwardDirections();
    }

    void driveForward(int speed) {
        driveMix(speed, speed, speed, speed);
    }

    void driveBackward(int speed) {
        driveMix(-speed, -speed, -speed, -speed);
    }

    void strafeLeft(int speed) {
        driveMix(-speed, speed, speed, -speed);
    }

    void strafeRight(int speed) {
        driveMix(speed, -speed, -speed, speed);
    }

    void driveDiagonalForwardLeft(int speed) {
        // Mostly left, with a small forward component.
        driveMix(-speed * 3 / 5, speed, speed, -speed * 3 / 5);
    }

    void driveDiagonalForwardRight(int speed) {
        driveMix(speed, 0, 0, speed);
    }

    void driveDiagonalBackwardLeft(int speed) {
        driveMix(0, -speed, -speed, 0);
    }

    void driveDiagonalBackwardRight(int speed) {
        driveMix(-speed, 0, 0, -speed);
    }

    // Positive wheel commands rotate the robot to the right with the assumed
    // wheel order. If the physical result is opposite, swap these two calls.
    void rotateYawLeft(int speed) {
        driveMix(-speed, speed, -speed, speed);
    }

    void rotateYawRight(int speed) {
        driveMix(speed, -speed, speed, -speed);
    }

    void stop() {
        for (uint8_t wheel = 0; wheel < WHEEL_COUNT; ++wheel) {
            analogWrite(WHEELS[wheel].enable_pin, 0);
            digitalWrite(WHEELS[wheel].in1_pin, LOW);
            digitalWrite(WHEELS[wheel].in2_pin, LOW);
        }
    }

private:
    void preloadForwardDirections() {
        for (uint8_t wheel = 0; wheel < WHEEL_COUNT; ++wheel) {
            digitalWrite(WHEELS[wheel].in1_pin, WHEELS[wheel].forward_in1_level);
            digitalWrite(WHEELS[wheel].in2_pin, WHEELS[wheel].forward_in2_level);
        }
    }

    void driveMix(int front_left, int front_right, int rear_left, int rear_right) {
        const int commands[WHEEL_COUNT] = {
            front_left,
            front_right,
            rear_left,
            rear_right
        };

        // Disable all wheels before changing direction signals.
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
        uint8_t in1_level = config.forward_in1_level;
        uint8_t in2_level = config.forward_in2_level;

        if (!forward) {
            in1_level = !in1_level;
            in2_level = !in2_level;
        }

        digitalWrite(config.in1_pin, in1_level);
        digitalWrite(config.in2_pin, in2_level);
        analogWrite(config.enable_pin, map(magnitude, 0, 100, 0, 255));
    }
};

const int STRAIGHT_SPEED = 20;
const int BACKWARD_SPEED = 35;
const int DIAGONAL_SPEED = 35;
const unsigned long DIAGONAL_DURATION_MS = 2000;
const unsigned long SERVO_SETTLE_DELAY_MS = 200;
const unsigned long DIRECTION_CHANGE_DELAY_MS = 200;
const uint8_t STICKER_SERVO_ID = 1;
const int16_t STICKER_CCW_SPEED = -3400;
const uint8_t STICKER_ACCELERATION = 0;

MecanumDrive drive;
SMS_STS sticker;

void beginSticker() {
    Serial.begin(1000000);
    sticker.pSerial = &Serial;
    delay(500);
}

void startStickerCCW() {
    sticker.WheelMode(STICKER_SERVO_ID);
    sticker.WriteSpe(STICKER_SERVO_ID, STICKER_CCW_SPEED, STICKER_ACCELERATION);
}

void stopSticker() {
    sticker.WriteSpe(STICKER_SERVO_ID, 0, STICKER_ACCELERATION);
}

void setup() {
    drive.begin();
    drive.stop();
    beginSticker();
}

void loop() {
    drive.stop();
    delay(1000);

    drive.driveForward(STRAIGHT_SPEED);
    delay(1000);

    drive.stop();
    delay(DIRECTION_CHANGE_DELAY_MS);

    startStickerCCW();
    drive.driveDiagonalForwardLeft(DIAGONAL_SPEED);
    delay(DIAGONAL_DURATION_MS);

    stopSticker();
    delay(SERVO_SETTLE_DELAY_MS);
    drive.stop();
    delay(DIRECTION_CHANGE_DELAY_MS);
    drive.driveBackward(BACKWARD_SPEED);
    delay(1000);

    drive.stop();
    while (true) {
        delay(1000);
    }
}
