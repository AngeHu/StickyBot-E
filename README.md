# StickyBot-E

Robot movement and sticker-servo demo for the ERH Physical AI Zurich Hackathon.

## Hardware

- Arduino Uno
- Two dual-channel motor drivers
- Four mecanum wheels
- Feetech serial servo on the Uno hardware UART

Motor wiring:

```text
Front board: ENA=9, IN1=8, IN2=10, IN3=12, IN4=13, ENB=11
Rear board:  ENA=3, IN1=2, IN2=4,  IN3=6,  IN4=7,  ENB=5
```

The servo bus uses pins D0/D1 at 1 Mbps. Disconnect the servo UART wiring while uploading firmware.

## Demo

[`StickyBot-E.ino`](StickyBot-E.ino) runs once and then stops:

1. Forward for 1 second at 20%
2. Left-forward diagonal for 4 seconds at 35%
3. Stop the sticker servo and disable its torque
4. Backward for 0.5 seconds at 20%
5. Stop

The servo uses ID 1, speed `-3400`, and acceleration `50`.

## Build and upload

Install the `SCServo` Arduino library, connect an Arduino Uno, and run:

```sh
arduino-cli compile --fqbn arduino:avr:uno .
arduino-cli upload --fqbn arduino:avr:uno --port PORT .
```
