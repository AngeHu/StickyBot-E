#include "src/Driver.h"

// Driver front(ENA_1, IN1_1, IN2_1, ENB_1, IN3_1, IN4_1);
// Driver rear(ENA_2, IN1_2, IN2_2, ENB_2, IN3_2, IN4_2);

// void setup() {
//     front.begin();
//     rear.begin();

//     front.set_direction(true);
//     rear.set_direction(true);
// }


// void loop() {
//     front.set_speed(50);
//     rear.set_speed(50);
// }

// SMS_STS st;
ServoDriver st;

void setup() {
    // Uno hardware UART, pins D0 and D1
    // Servo default baud rate: 1 Mbps
    // st.pSerial = &Serial;
    st.begin();
    delay(1000);
}

void loop()
{
    // st.move_sticker(90);
    // delay(2000);
    // st.move_sticker(0);
    // delay(2000);

    // st.tilt_head(45);
    // delay(2000);
    // st.tilt_head(0);
    // delay(2000);

    // st.move_eyes(90);
    // delay(2000);
    // st.move_eyes(0);
    // delay(2000);
}


// #include <SCServo.h>

// SMS_STS st;

// int ID_ChangeFrom = 1;   // The original ID of the servo whose ID is to be changed. The factory default is 1
// int ID_Changeto   = 2;   // The new ID
// void setup() {
//   Serial.begin(1000000);
//   st.pSerial = &Serial;
//   while(!Serial) {}

//   st.unLockEprom(ID_ChangeFrom); //Unlock EPROM-SAFE
//   st.writeByte(ID_ChangeFrom, SMS_STS_ID, ID_Changeto);//Change ID
//   st.LockEprom(ID_Changeto); // EPROM-SAFE locked
// }

// void loop() {
// }