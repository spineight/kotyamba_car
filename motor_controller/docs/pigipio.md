## extern/pigipio (pigipio)
https://github.com/guymcswain/pigpio-client/wiki/Install-and-configure-pigpiod

The initial part of the make, the compilation of pigpio.c,
takes 100 seconds on early model Pis.  Be patient.  The overall
install takes just over 3 minutes.

```bash
cd extern/pigpio/
make
sudo make install
```