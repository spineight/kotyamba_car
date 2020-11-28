## /extern/wiringPi (WiringPi)
To use the library the src code needs to be built and installed.

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