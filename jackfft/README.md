jackfft - an audio filtering application using JACK (http://jackaudio.org/)
=========
To build:
```
cd workspace
make bin/jackfft # with gtk GUI
make bin/jackfftc # command line version
```
Dependencies
```
libjack-jackd2-dev libfftw3-dev libsoundtouch-dev libgtkmm-2.4-dev libglibmm-2.4-dev libcairomm-1.0-dev
```
Only the first two are needed for jackfftc.

To use:
```
cd workspace
. set_env
jackfft
```
