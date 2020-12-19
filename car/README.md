Configuring program to run on the boot:

https://linuxhint.com/run_linux_command_script_sys_reboot/
https://www.simplified.guide/linux/automatically-run-program-on-startup

Using cron

```bash
sudo crontab -e
```

add this line:
```bash
@reboot sleep 5 && /bin/echo -e 'connect 1C:A0:B8:E4:ED:76 \n quit \n' | bluetoothctl && sleep 30 && /home/ubuntu//dev/kotyamba_car/release/car/test_car
```
1. /bin/echo -e 'connect 1C:A0:B8:E4:ED:76 \n quit \n' | bluetoothctl
to start bluetoothctl service and connect to the joystick with MAC address 1C:A0:B8:E4:ED:76
2. sleep 30 - wait enough time for the connection to be established
3. start test_car application
