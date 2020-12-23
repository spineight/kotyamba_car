#include <car.hpp>
namespace kotyamba {
  void Car::driving_loop() {
    //@TODO consider setting thread affinity
//    std::cout << "#### Setting driving_loop thread`s affinity programatically #####\n";
//    cpu_set_t cpuset;
//    CPU_ZERO(&cpuset);
//    constexpr int CPU_ID = 2;
//    CPU_SET(CPU_ID, &cpuset);
//    const int rc = pthread_setaffinity_np(pthread_self(),
//                                    sizeof(cpu_set_t), &cpuset);
//    if (rc != 0) {
//      std::cerr << "Error calling pthread_setaffinity_np: " << rc << "\n";
//    }

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
    const double DEAD_ZONE_LENGHT = 5000;

    enum BUTTON_STATE {
      OFF, ON
    };
    const size_t X_button_ID{0}, O_button_ID{1};

    bool is_running = true;

    while (is_running) {
      //@TODO consider using enabling this for log ?
//      std::cout << "car driving loop on CPU" << sched_getcpu() << '\n';
      //! for button 0/1
      const bool is_x_btn_pressed = joystick.button0_val.load(std::memory_order_relaxed);
      const bool is_o_btn_pressed = joystick.button1_val.load(std::memory_order_relaxed);
      if (is_o_btn_pressed) {
        throttle_motor.emergency_stop();
        steering_motor.emergency_stop();
        continue;
      }
      if (is_x_btn_pressed) {
        is_running = false;
        std::cout << "stopping\n";
        joystick.stop_event_loop();
        break;
      }

      //! value for axis - is a signed integer between -32767 and +32767
      const double x = joystick.axis1_val.load(std::memory_order_relaxed);
      const double y = joystick.axis2_val.load(std::memory_order_relaxed);
//      std::cout << "x:" << x << ", y:" << y << '\n';

      const double abs_throttle_normalized = abs(x) / MAX_OFFSET;
      const double abs_steering_normalized = abs(y) / MAX_OFFSET;

      if (y >= 0 + DEAD_ZONE_LENGHT) {
        steering_motor.rotate(abs_steering_normalized, MotorController::BACKWARD_ROTATION);
      } else if (y <= 0 - DEAD_ZONE_LENGHT) {
        steering_motor.rotate(abs_steering_normalized, MotorController::FORWARD_ROTATION);
      }

      if (x >= 0 + DEAD_ZONE_LENGHT) {
        throttle_motor.rotate(abs_throttle_normalized, MotorController::BACKWARD_ROTATION);
      } else if (x <= 0 - DEAD_ZONE_LENGHT) {
        throttle_motor.rotate(abs_throttle_normalized, MotorController::FORWARD_ROTATION);
      }

      if (abs(x) < DEAD_ZONE_LENGHT && abs(y) < DEAD_ZONE_LENGHT) {
        throttle_motor.stop();
        steering_motor.stop();
      }
    }
  }
}

int main() {
  kotyamba::Car car;
}