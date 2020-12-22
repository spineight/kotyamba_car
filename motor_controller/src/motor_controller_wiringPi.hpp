#pragma once

#include <motor_controller.hpp>

#include <wiringPi.h>
#include <softPwm.h>

#include <iostream>

namespace kotyamba {
  static bool was_init_function_called = false;

  MotorController::MotorController(char direction_pin_0, char direction_pin_1, char speed_pin, size_t pwm_range, size_t frequency)
          : direction_pin_0(direction_pin_0),
            direction_pin_1(direction_pin_1),
            speed_pin(speed_pin),
            pwm_range(pwm_range),
            frequency(frequency)
  {
    std::cout << "WiringPI is used\n";
    if(!was_init_function_called) {
      if(wiringPiSetupGpio() == -1) {
        std::cerr << "failed to setup WiringPi\n";
        exit(1);
      }
      was_init_function_called = true;
    }


    pinMode(direction_pin_0, OUTPUT);
    pinMode(direction_pin_1, OUTPUT);

    // ### http://wiringpi.com/reference/software-pwm-library/
    // https://raspberrypi.stackexchange.com/questions/23361/wiringpi-softpwm-calculation-to-determine-duty-cycle
    // The basic pulse unit is 100 micros long.
    //
    // The final paramater in softPwmCreate says how long each cycle is in basic pulse units.
    //
    // softPwmCreate(1,0,200) says create a cycle 20 ms long made up of 200 steps (20 ms as 200 * 100 = 20000 microseconds).
    // softPwmWrite(1,185) says keep the pulse high for 18.5 ms in every 20 ms cycle (18.5 ms as 185 * 100 = 18500 microseconds). That is way too long. Servos nominally respond to 1 ms - 2 ms pulses. So you should choose values 10 - 20.
    //
    // softPwmCreate(1,0,500) says create a cycle 50 ms long made up of 500 steps.
    //
    // softPwmWrite(1,14) says keep the pulse high for 1.4 ms in every 50 ms cycle. This will be close to the central position. A correct value would be 15 (again nominally values 10-20 should be used).
    //
    // The 20 ms cycle is more correct for servos as it will give a nominal update of 50 Hz. A 50 ms cycle will only update at 20 Hz.
    const int INITIAL_VALUE = 0;
    softPwmCreate(speed_pin, INITIAL_VALUE, pwm_range);
    // Notes
    //
    //    Each “cycle” of PWM output takes 10mS with the default range value of 100, so trying to change the PWM value more than 100 times a second will be futile.
    //    Each pin activated in softPWM mode uses approximately 0.5% of the CPU.
    //    There is currently no way to disable softPWM on a pin while the program in running.
    //    You need to keep your program running to maintain the PWM output!
  }
  MotorController::~MotorController() {}

  /*!
   *
   * @param duty_cycle [0..1] - fraction of power (1 - 100%, 0 - 0%)
   * @param d - FORWARD, BACKWARD
   */
  void MotorController::rotate(double duty_cycle, State new_state) {
    // enable forward/backward rotation
    if(new_state == State::FORWARD_ROTATION && state != FORWARD_ROTATION) {
      digitalWrite(direction_pin_0, HIGH);
      digitalWrite(direction_pin_1, LOW);
      state = FORWARD_ROTATION;
    }
    else if(new_state == State::BACKWARD_ROTATION && state != BACKWARD_ROTATION) {
      digitalWrite(direction_pin_0, LOW);
      digitalWrite(direction_pin_1, HIGH);
      state = BACKWARD_ROTATION;
    }
    // For the lone PWM pin, you can use pwmWrite([pin], [0-1023]) to set it to a value between 0 and 1024.
    softPwmWrite(speed_pin, pwm_range * duty_cycle);
  }

  void MotorController::stop() {
    softPwmWrite(speed_pin, 0);
    state = NO_ROTATION;
  }

  void MotorController::emergency_stop() {
    softPwmWrite(speed_pin, 0);
    digitalWrite(direction_pin_0, LOW);
    digitalWrite(direction_pin_1, LOW);
    state = NO_ROTATION;
  }
}