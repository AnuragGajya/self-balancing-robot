# self-balancing-robot
"Arduino + MPU6050 + L298N self-balancing robot with PID control"

# Self-Balancing Robot 🤖

Built a self-balancing robot that combines control theory, sensor fusion, and motor control into one satisfying (and occasionally hilarious) feedback loop — a two-wheeled robot that balances itself upright using PID control.

## How it works

The robot constantly measures its tilt angle using an MPU6050 IMU, compares it against an upright "setpoint," and feeds the error into a PID controller. The output drives a pair of DC motors through an L298N motor driver — pushing the robot back toward vertical dozens of times per second, the same basic principle behind a Segway.

## Core components & connections

**MPU6050 (gyroscope + accelerometer) → Arduino**
- SDA → A4
- SCL → A5
- INT → Digital Pin 2 (interrupt-driven motion processing via the DMP)
- VCC → 5V, GND → GND

**Arduino → L298N motor driver**
- Pin 6 → IN1
- Pin 9 → IN2
- Pin 10 → IN3
- Pin 11 → IN4
- (Enable A / Enable B tied HIGH to activate both channels)

**L298N → Motors & power**
- OUT1/OUT2 → Motor A
- OUT3/OUT4 → Motor B
- 12V in → Battery pack
- Common GND across Arduino, MPU6050, L298N, and battery

## The fun part — tuning

Getting this to actually stand up isn't just wiring, it's PID tuning by feel: start with just Kp until it fights back (but shakes), add Kd to settle the shake, then a touch of Ki if it drifts. Classic control systems trial-and-error, just with a robot faceplanting onto your desk every few seconds until it clicks 😄

Calibrating the gyroscope offsets and finding the exact "upright" setpoint angle from the serial monitor made all the difference between a robot that balances and one that just falls over with more PWM.

A small project, but a great hands-on intro to feedback control, sensor interrupts, and motor driving — all the fundamentals robotics engineers use at a much larger scale.

#Arduino #Robotics #EmbeddedSystems #PIDControl #Electronics #MakerProjects #IoT #Engineering
