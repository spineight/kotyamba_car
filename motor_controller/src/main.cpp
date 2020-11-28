#include <motor_controller.hpp>
#include <iostream>

// http://www.cs.ukzn.ac.za/~hughm/os/notes/ncurses.html
#include <curses.h>

int main() {

  const char THROTTLE_MOTOR_pin0 = 7;
  const char THROTTLE_MOTOR_pin1 = 8;
  const char THROTTLE_MOTOR_EnA_pin = 1;

  const char STEERING_MOTOR_pin0 = 9;
  const char STEERING_MOTOR_pin1 = 10;
  const char STEERING_MOTOR_EnB_pin = 11;

  const size_t PWM_RANGE = 255;
  const size_t FREQUENCY = 20000;

  kotyamba::MotorController throttle_motor(THROTTLE_MOTOR_pin0, THROTTLE_MOTOR_pin1, THROTTLE_MOTOR_EnA_pin, PWM_RANGE, FREQUENCY);
  kotyamba::MotorController steering_motor(STEERING_MOTOR_pin0, STEERING_MOTOR_pin1, STEERING_MOTOR_EnB_pin, PWM_RANGE, FREQUENCY);

  bool is_running = true;

  // http://www.cs.ukzn.ac.za/~hughm/os/notes/ncurses.html
  //  The very first thing to do: Before you use any other curses routines, the initscr() routine must be called first.
  initscr();  // Init and go to ncurses mode
  // One-character-a-time.
  // To disable the buffering of typed characters by the TTY driver and get a character-at-a-time input, you need to call
  cbreak();
  // No echo. To suppress the automatic echoing of typed characters, you need to call
  noecho();

  printw("w- up, s - down, a - left, d - right;   b - stop, x - hard break, q - quit");

  while(is_running) {
    const char command = tolower(getch()); // ncurses
    const double throttle_duty_cycle = PWM_RANGE;
    const double steering_duty_cycle = PWM_RANGE * 0.8;
    switch (command) {
      case 'w': throttle_motor.rotate(throttle_duty_cycle, kotyamba::MotorController::FORWARD); break;
      case 's': throttle_motor.rotate(throttle_duty_cycle, kotyamba::MotorController::BACKWARD); break;
      case 'a': steering_motor.rotate(steering_duty_cycle, kotyamba::MotorController::FORWARD); break;
      case 'd': steering_motor.rotate(steering_duty_cycle, kotyamba::MotorController::BACKWARD); break;
      case 'b': steering_motor.stop();
      throttle_motor.stop();
      break;
      case 'x': steering_motor.emergency_stop();
      throttle_motor.emergency_stop();
      break;
      case 'q': steering_motor.stop();
      throttle_motor.stop();
      is_running = false;
      break;
    }
  }
  struct ScreenGuard {

    ~ScreenGuard() { endwin(); };  // Free memory switch to ordinary mode (non ncurses)
  } guard;

  return 0;
}
