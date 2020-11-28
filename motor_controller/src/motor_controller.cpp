#ifdef WITH_WIRINGPI
  #include "motor_controller_wiringPi.hpp"
#elif defined WITH_PIGPIO
  #include "motor_controller_pigpio.hpp"
#else
  #error "One of gpio libraries should be present"
#endif

