if signal on pins:
        directionPinFirst_GPIO_id - TRUE, directionPinSecond_GPIO_id - FALSE - motor rotates in one direction
        directionPinFirst_GPIO_id - FALSE, directionPinSecond_GPIO_id - TRUE - rotates in oposite direction
        when signals are TRUE & TRUE or FALSE & FALSE - motor is stoped
        when jumper from enB or enA is removed - we can specify voltage and use PWM for speed control,
        otherwise motor will rotate with max speed

        dc is the duty cycle (0.0% <= dc <= 100.0%)


Motor is blocked
        from 298 datasheet three (IN1, IN2, EN) pins for each motor channel.
        EN=0 => freewheel
        else EN=1
        IN1=1, IN2=0 => power forward
        IN1=0, IN2=1 => power backward
        IN1=IN2=1, or IN1=IN2=0 => power braking (motor is shorted through either the high-side or low-side transistors)
        https://forum.arduino.cc/index.php?topic=235720.0


0 ... 100%
        the duty cycle (0.0 <= dc <= 100.0) is used in GPIO.PWM
        for refs: https://sourceforge.net/p/raspberry-gpio-python/wiki/PWM/


https://learn.sparkfun.com/tutorials/raspberry-gpio/c-wiringpi-setup

https://projects.drogon.net/raspberry-pi/wiringpi/download-and-install/


## Building wiringpi
```bash
cd wiringPi
./build
```

## Using wiringPi
NOTE: To compile programs with wiringPi, you need to add:
    -lwiringPi
  to your compile line(s) To use the Gertboard, MaxDetect, etc.
  code (the devLib), you need to also add:
    -lwiringPiDev
  to your compile line(s).

## Testing wiringPi installation
```bash
run the gpio command to check the installation:

gpio -v
gpio readall
```

WiringPi is awesome because it's actually more than just a C library,
it includes a command-line utility as well! You can test your installation of WiringPi with the gpio utility.

Open up a terminal, and try some of these system calls:

```bash
pi@raspberrypi ~/code $ gpio -g mode 18 output
pi@raspberrypi ~/code $ gpio -g write 18 1
pi@raspberrypi ~/code $ gpio -g write 18 0
```
As long as your LED is still connected to pin 18 it should blink on and off following the last two commands.

Or, to test the button, type:
```bash
pi@raspberrypi ~/code $ gpio -g mode 17 up
pi@raspberrypi ~/code $ gpio -g read 17
```
Either 0 or 1 will be returned, depending on whether the button is pressed or not.
Try typing that last line again while pressing the button.

The gpio utility, as stated in the manual, is a "swiss army knife" command-line tool.
We highly recommend checking out the man page (type man gpio) to discover everything it can do.

