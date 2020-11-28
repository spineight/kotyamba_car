TB6612FNG

### Wires colors
gray PWMB
white BIN2
blue BIN1

green PWMA
orange AIN2
yellow AIN1

purple - STBY


### Specification:
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