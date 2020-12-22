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

  const size_t PWM_RANGE = 255;
  const size_t FREQUENCY = 20000;
  struct Car {
    std::thread joystick_even_loop_thread;
    Car()
    : throttle_motor(THROTTLE_MOTOR_pin0, THROTTLE_MOTOR_pin1, THROTTLE_MOTOR_EnA_pin, PWM_RANGE, FREQUENCY),
      steering_motor(STEERING_MOTOR_pin0, STEERING_MOTOR_pin1, STEERING_MOTOR_EnB_pin, PWM_RANGE, FREQUENCY)
    {
      joystick.init();
      joystick_even_loop_thread = std::thread([this](){joystick.event_loop();});
      driving_loop();
    }
    ~Car() {
      joystick_even_loop_thread.join();
    }
    //! value for axis - is a signed integer between -32767 and +32767
    //! for button 0/1
    //! if returns false - exit joystick event loop
    void driving_loop() {
      /*
       *     _____> Y
       *     |
       *     |
       *     |
       *     V X
       */
      const size_t X_AXIS_ID = 1;
      const size_t Y_AXIS_ID = 2;
      const double MAX_OFFSET = 32767;
      const double DEAD_ZONE_LENGHT = 1000;

      enum BUTTON_STATE{OFF,ON};
      const size_t X_button_ID{0}, O_button_ID{1};

      bool is_running = true;
      while(is_running) {


        const bool is_x_btn_pressed = joystick.is_button_pressed(X_button_ID);
        const bool is_o_btn_pressed = joystick.is_button_pressed(O_button_ID);
        if(is_o_btn_pressed) {
          throttle_motor.emergency_stop();
          steering_motor.emergency_stop();
          continue;
        }
        if(is_x_btn_pressed) {
          is_running = false;
          std::cout << "stopping\n";
          joystick.stop_event_loop();
          break;
        }

        const double x = joystick.get_axis_value(X_AXIS_ID);
        const double y = joystick.get_axis_value(Y_AXIS_ID);

        const double abs_throttle_normalized = abs(x) / MAX_OFFSET;
        const double abs_steering_normalized = abs(y) / MAX_OFFSET;

        if(y >= 0 + DEAD_ZONE_LENGHT) {
//          std::cout << "abs_steering_normalized:" << abs_steering_normalized <<'\n';
          steering_motor.rotate(abs_steering_normalized, MotorController::BACKWARD_ROTATION);
        }
        else if(y <= 0 - DEAD_ZONE_LENGHT) {
//          std::cout << "abs_steering_normalized:" << abs_steering_normalized <<'\n';
          steering_motor.rotate(abs_steering_normalized, MotorController::FORWARD_ROTATION);
        }

        if(x >= 0 + DEAD_ZONE_LENGHT) {
//          std::cout << "abs_throttle_normalized:" << abs_throttle_normalized << '\n';
          throttle_motor.rotate(abs_throttle_normalized, MotorController::BACKWARD_ROTATION);
        }
        else if(x <= 0 - DEAD_ZONE_LENGHT) {
//          std::cout << "abs_throttle_normalized:" << abs_throttle_normalized << '\n';
          throttle_motor.rotate(abs_throttle_normalized, MotorController::FORWARD_ROTATION);
        }

        if(x < DEAD_ZONE_LENGHT && y < DEAD_ZONE_LENGHT ) {
          throttle_motor.stop();
          steering_motor.stop();
        }
      }
    }
    Joystick joystick;
    MotorController throttle_motor;
    MotorController steering_motor;
  };
}