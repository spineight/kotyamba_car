#pragma once

#include <motor.hpp>

#include <pigpio.h>

#include <iostream>

// https://github.com/joan2937/pigpio
// http://abyz.me.uk/rpi/pigpio/cif.html

namespace kotyamba {
  static bool was_init_function_called = false;

  Motor::Motor(char direction_pin_0, char direction_pin_1, char speed_pin, size_t pwm_range, size_t frequency)
          : direction_pin_0(direction_pin_0),
            direction_pin_1(direction_pin_1),
            speed_pin(speed_pin),
            pwm_range(pwm_range),
            frequency(frequency)
  {
    if (gpioInitialise() < 0) {
      std::cerr << "failed to setup WiringPi\n";
      exit(1);
    }

    gpioSetMode(direction_pin_0, PI_OUTPUT);
    gpioSetMode(direction_pin_1, PI_OUTPUT);
    gpioSetMode(speed_pin, PI_OUTPUT);

    //!
    // Selects the dutycycle range to be used for the GPIO.
    // Subsequent calls to gpioPWM will use a dutycycle between 0 (off) and range (fully on).
//    user_gpio: 0-31
//    range: 25-40000
//
//
//    Returns the real range for the given GPIO's frequency if OK, otherwise PI_BAD_USER_GPIO or PI_BAD_DUTYRANGE.
//
//    If PWM is currently active on the GPIO its dutycycle will be scaled to reflect the new range.
//
//    The real range, the number of steps between fully off and fully on for each frequency, is given in the following table.
//
//    25,   50,  100,  125,  200,  250,  400,   500,   625,
//            800, 1000, 1250, 2000, 2500, 4000, 5000, 10000, 20000
    const int code = gpioSetPWMrange(speed_pin, pwm_range);
    if(code == PI_BAD_USER_GPIO) {
      std::cerr << "PI_BAD_USER_GPIO\n";
      exit(1);
    }
    if(code == PI_BAD_DUTYRANGE) {
      std::cerr << "PI_BAD_DUTYRANGE\n";
      exit(1);
    }

    // Returns the numerically closest frequency if OK, otherwise PI_BAD_USER_GPIO.
    //
    //If PWM is currently active on the GPIO it will be switched off and then back on at the new frequency.
    //
    //Each GPIO can be independently set to one of 18 different PWM frequencies.
    //
    //The selectable frequencies depend upon the sample rate which may be 1, 2, 4, 5, 8, or 10 microseconds (default 5).
    //
    //The frequencies for each sample rate are:
    //
    //                       Hertz
    //
    //       1: 40000 20000 10000 8000 5000 4000 2500 2000 1600
    //           1250  1000   800  500  400  250  200  100   50
    //
    //       2: 20000 10000  5000 4000 2500 2000 1250 1000  800
    //            625   500   400  250  200  125  100   50   25
    //
    //       4: 10000  5000  2500 2000 1250 1000  625  500  400
    //            313   250   200  125  100   63   50   25   13
    //sample
    // rate
    // (us)  5:  8000  4000  2000 1600 1000  800  500  400  320
    //            250   200   160  100   80   50   40   20   10
    //
    //       8:  5000  2500  1250 1000  625  500  313  250  200
    //            156   125   100   63   50   31   25   13    6
    //
    //      10:  4000  2000  1000  800  500  400  250  200  160
    //            125   100    80   50   40   25   20   10    5
    //
    //
    //Example
    //
    //gpioSetPWMfrequency(23, 0); // Set GPIO23 to lowest frequency.
    //
    //gpioSetPWMfrequency(24, 500); // Set GPIO24 to 500Hz.
    //
    //gpioSetPWMfrequency(25, 100000); // Set GPIO25 to highest frequency.
    if(gpioSetPWMfrequency(speed_pin, frequency) == PI_BAD_USER_GPIO) {
      std::cerr << "PI_BAD_USER_GPIO\n";
      exit(1);
    }
  }
  Motor::~Motor() {
    gpioTerminate();
  }

  /*!
   *
   * @param duty_cycle [0..1] - fraction of power (1 - 100%, 0 - 0%)
   * @param d - FORWARD, BACKWARD
   */
  void Motor::rotate(size_t duty_cycle, Direction d) {
    // enable forward/backward rotation
    if(d == Direction::FORWARD) {
      gpioWrite(direction_pin_0, 1);
      gpioWrite(direction_pin_1, 0);
    }
    else {
      gpioWrite(direction_pin_0, 0);
      gpioWrite(direction_pin_1, 1);
    }
    // For the lone PWM pin, you can use pwmWrite([pin], [0-1023]) to set it to a value between 0 and 1024.
    const int ret = gpioPWM(speed_pin, duty_cycle);
    if(ret == PI_BAD_USER_GPIO) {
      std::cerr << "PI_BAD_USER_GPIO\n";
      exit(1);
    }
    else if(ret == PI_BAD_DUTYCYCLE) {
      std::cerr << "PI_BAD_DUTYCYCLE\n";
      exit(1);
    }
  }

  void Motor::stop() {
    gpioPWM(speed_pin, 0);
  }

  void Motor::emergency_stop() {
    gpioPWM(speed_pin, 0);
    gpioWrite(direction_pin_0, 0);
    gpioWrite(direction_pin_1, 0);
  }
}