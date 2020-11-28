## refs
https://www.raspberrypi.org/documentation/hardware/raspberrypi/gpio/README.md
https://www.raspberrypi.org/documentation/usage/gpio/README.md

## GPIO pinout
```bash
sudo apt install python3-gpiozero
pinout
```

NOTE: To compile programs with wiringPi, you need to add:
    -lwiringPi
  to your compile line(s) To use the Gertboard, MaxDetect, etc.
  code (the devLib), you need to also add:
    -lwiringPiDev
  to your compile line(s).


## using wiringPi
```c++
#include <wiringPi.h>
```

After you've included the library, your first steps should be to initialize it.
This step also determines which pin numbering scheme you'll be using throughout the rest of your program.
Pick one of these function calls to initialize the library:
```c++
wiringPiSetup(); // Initializes wiringPi using wiringPi's simlified number system.
wiringPiSetupGpio(); // Initializes wiringPi using the Broadcom GPIO pin numbers
```

WiringPi's simplified number system introduces a third pin-numbering scheme.
We didn't show it in the table earlier, if you want to use this scheme, check out their pins page for an overview.
(https://projects.drogon.net/raspberry-pi/wiringpi/pins/)

Pin Mode Declaration

To set a pin as either an input or output, use the pinMode([pin], [mode]) function.
Mode can be either INPUT, OUTPUT, or PWM_OUTPUT.

For example, to set pin 22 as an input, 23 as an output, and 18 as a PWM, write:
```c++
wiringPiSetupGpio()
pinMode(17, INPUT);
pinMode(23, OUTPUT);
pinMode(18, PWM_OUTPUT);
```

Digital Output

The digitalWrite([pin], [HIGH/LOW]) function can be used to set an output pin either HIGH or LOW.
Easy enough, if you're an Arduino user.

To set pin 23 as HIGH, for example, simply call:
```c++
digitalWrite(23, HIGH);
```

PWM ("Analog") Output

For the lone PWM pin, you can use pwmWrite([pin], [0-1023]) to set it to a value between 0 and 1024. As an example...
```c++
pwmWrite(18, 723);
```
...will set pin 18 to a duty cycle around 70%.

Digital Input

If you're an Arduino veteran, you probably know what comes next.
To read the digital state of a pin, digitalRead([pin]) is your function. For example...
```c++
if (digitalRead(17))
    printf("Pin 17 is HIGH\n");
else
    printf("Pin 17 is LOW\n");
```
...will print the status of pin 22. The digitalRead() function returns 1 if the pin is HIGH and 0 if it's LOW.

Pull-Up/Down Resistors

Need some pull-up or pull-down resistors on your digital input? Use the pullUpDnControl([pin], [PUD_OFF, PUD_DOWN, PUD_UP]) function to pull your pin.

For example, if you have a button on pin 22 and need some help pulling it up, write:
```c++
pullUpDnControl(17, PUD_UP);
```
That comes in handy if your button pulls low when it's pressed.

Delays

Slowing down those blinking LEDs is always useful -- assuming you actually want to differentiate between on and off.
WiringPi includes two delay functions to choose from: delay([milliseconds]) and delayMicroseconds([microseconds]).
The standard delay will halt the program flow for a specified number of milliseconds.
If you want to delay for 2 seconds, for example, write:
```c++
delay(2000);
```



http://wiringpi.com/reference/core-functions/


### void pinMode (int pin, int mode) ;

This sets the mode of a pin to either INPUT, OUTPUT, PWM_OUTPUT or GPIO_CLOCK.
Note that only wiringPi pin 1 (BCM_GPIO 18) supports PWM output and only wiringPi pin 7 (BCM_GPIO 4) supports CLOCK output modes.

This function has no effect when in Sys mode.
If you need to change the pin mode, then you can do it with the gpio program in a script before you start your program.


### void digitalWrite (int pin, int value) ;

Writes the value HIGH or LOW (1 or 0) to the given pin which must have been previously set as an output.

WiringPi treats any non-zero number as HIGH, however 0 is the only representation of LOW.

###  void pwmWrite (int pin, int value) ;

Writes the value to the PWM register for the given pin.
The Raspberry Pi has one on-board PWM pin, pin 1 (BMC_GPIO 18, Phys 12) and the range is 0-1024.
Other PWM devices may have other PWM ranges.

This function is not able to control the Pi’s on-board PWM when in Sys mode.



    int digitalRead (int pin) ;

This function returns the value read at the given pin. It will be HIGH or LOW (1 or 0) depending on the logic level at the pin.

    analogRead (int pin) ;

This returns the value read on the supplied analog input pin. You will need to register additional analog modules to enable this function for devices such as the Gertboard, quick2Wire analog board, etc.

    analogWrite (int pin, int value) ;

This writes the given value to the supplied analog pin. You will need to register additional analog modules to enable this function for devices such as the Gertboard.