#pragma once
#include <joystick.hpp>
#include <motor_controller.hpp>

#include <cmath>
#include <thread>
#include <iostream>

namespace kotyamba {
  static const char THROTTLE_MOTOR_pin0 = 7;
  static const char THROTTLE_MOTOR_pin1 = 8;
  static const char THROTTLE_MOTOR_EnA_pin = 1;

  static const char STEERING_MOTOR_pin0 = 9;
  static const char STEERING_MOTOR_pin1 = 10;
  static const char STEERING_MOTOR_EnB_pin = 11;

  // STDBY wire is connected to 3V3 Pin

#ifdef WITH_WIRINGPI
  const size_t PWM_RANGE = 255;
  const size_t FREQUENCY = 20000;
#elif defined WITH_PIGPIO
  const size_t PWM_RANGE = 100;
  const size_t FREQUENCY = 100;
#else
#error "One of gpio libraries should be present"
#endif


  struct Car {
    std::thread joystick_event_loop_thread;
    Car()
    : throttle_motor(THROTTLE_MOTOR_pin0, THROTTLE_MOTOR_pin1, THROTTLE_MOTOR_EnA_pin, PWM_RANGE, FREQUENCY),
      steering_motor(STEERING_MOTOR_pin0, STEERING_MOTOR_pin1, STEERING_MOTOR_EnB_pin, PWM_RANGE, FREQUENCY)
    {
      joystick.init();
      joystick_event_loop_thread = std::thread([this](){joystick.event_loop();});
      driving_loop();
    }
    ~Car() {
      if(joystick_event_loop_thread.joinable())
        joystick_event_loop_thread.join();
    }

    void driving_loop();
    Joystick joystick;
    MotorController throttle_motor;
    MotorController steering_motor;
  };
}