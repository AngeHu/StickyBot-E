#ifndef DRIVER_H
#define DRIVER_H

#include <SCServo.h>

#define ENA_1 9
#define IN1_1 7
#define IN2_1 8
#define ENB_1 3
#define IN3_1 2
#define IN4_1 4

#define ENA_2 10
#define IN1_2 5
#define IN2_2 6
#define ENB_2 11
#define IN3_2 13
#define IN4_2 12


#define STICKER_SERVO 1
#define PITCH_SERVO 2 // head tilt
#define ROLL_SERVO 3  // eyes motions

class DCDriver {
    int ena_pin;
    int in1_pin;
    int in2_pin;
    int enb_pin;
    int in3_pin;
    int in4_pin;

    public:
    DCDriver(int ena, int in1, int in2, int enb, int in3, int in4);
    bool begin();
    int to_speed(int speed);
    bool set_speed(int speed);
    bool set_direction(bool forward);
    bool stop();
};

class ServoDriver {
    static SMS_STS st;

    public:
    ServoDriver();
    bool begin();

    // bool move_head(int tilt_angle);
    bool move_sticker(int angle);
    bool tilt_head(int angle);
    bool move_eyes(int angle);

    int to_angle(int angle) {
        return map(angle, 0, 180, 0, 4095);
    }
};

#endif // DRIVER_H