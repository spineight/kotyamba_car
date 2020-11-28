#pragma once

#include <cstddef>

namespace kotyamba {

  struct MotorController {
    /*! from 298 datasheet three (IN1, IN2, EN) pins for each motor channel.
    EN=0 => freewheel
    else EN=1
    IN1=1, IN2=0 => power forward
    IN1=0, IN2=1 => power backward
    IN1=IN2=1, or IN1=IN2=0 => power braking (motor is shorted through either the high-side or low-side transistors)
    https://forum.arduino.cc/index.php?topic=235720.0
   */
    MotorController(char direction_pin_0, char direction_pin_1, char speed_pin, size_t pwm_range, size_t frequency);
    ~MotorController();
    enum Direction {FORWARD, BACKWARD};
    void rotate(size_t duty_cycle, Direction d);
    void stop();
    void emergency_stop();

    char direction_pin_0, direction_pin_1, speed_pin;
    size_t pwm_range, frequency;
  };
};