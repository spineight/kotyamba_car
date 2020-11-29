#pragma once
#include <joystick.hpp>
#include <motor_controller.hpp>

namespace kotyamba {
  static const char THROTTLE_MOTOR_pin0 = 7;
  static const char THROTTLE_MOTOR_pin1 = 8;
  static const char THROTTLE_MOTOR_EnA_pin = 1;

  static const char STEERING_MOTOR_pin0 = 9;
  static const char STEERING_MOTOR_pin1 = 10;
  static const char STEERING_MOTOR_EnB_pin = 11;

  const size_t PWM_RANGE = 255;
  const size_t FREQUENCY = 20000;
  struct Car {
    Car()
    : throttle_motor(THROTTLE_MOTOR_pin0, THROTTLE_MOTOR_pin1, THROTTLE_MOTOR_EnA_pin, PWM_RANGE, FREQUENCY),
      steering_motor(STEERING_MOTOR_pin0, STEERING_MOTOR_pin1, STEERING_MOTOR_EnB_pin, PWM_RANGE, FREQUENCY)
    {
      auto event_handler_fn =[this](uint32_t timestamp,
                                    const std::vector<short>& axes,
                                    const std::vector<short>& buttons ) {
        return on_joystick_event(timestamp, axes, buttons);
      };

      joystick.set_EventHandler(event_handler_fn);
      joystick.init();
      joystick.event_loop();
    }
//    //! value for axis - is a signed integer between -32767 and +32767
//    //! for button 0/1
//    using on_event_FnT = std::function<void(uint32_t/*event timestamp in milliseconds*/,
//                                            const std::vector<short>& /*axes*/,
//                                            const std::vector<short>& /*buttons*/)>;
    bool on_joystick_event(uint32_t/*event timestamp in milliseconds*/,
                           const std::vector<short>& axes,
                           const std::vector<short>& buttons) {
      const size_t x_axis_id = 3;
      const size_t y_axis_id = 4;
      const short x = axes[x_axis_id];
      const short y = axes[y_axis_id];
      const int MAX_OFFSET = 32767;

      const double throttle_duty_cycle = PWM_RANGE;
      const double steering_duty_cycle = PWM_RANGE * 0.8;

      const double EPSILON = 100;

      if(y > 0 + EPSILON)
        throttle_motor.rotate(throttle_duty_cycle * y / MAX_OFFSET, MotorController::FORWARD);
      else if(y < 0 - EPSILON)
        throttle_motor.rotate(throttle_duty_cycle * abs(y) / MAX_OFFSET, MotorController::BACKWARD);

      if(x > 0 + EPSILON)
        steering_motor.rotate(steering_duty_cycle * x / MAX_OFFSET, MotorController::FORWARD);
      else if(x < 0 - EPSILON)
        steering_motor.rotate(steering_duty_cycle * abs(x) / MAX_OFFSET, MotorController::BACKWARD);

      if(x < EPSILON && y < EPSILON ) {
        throttle_motor.stop();
        steering_motor.stop();
      }

      const size_t X_button_ID = 0;
      enum BUTTON_STATE{OFF,ON};
      if(buttons[X_button_ID] == ON) {
        std::cout << "closing\n";
        return false;
      }
      return true;
    }
    Joystick joystick;
    MotorController throttle_motor;
    MotorController steering_motor;
  };
}