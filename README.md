# Mycotronics ESP8266
=====================

```bash
apt install platformio
```
clone this repo and cd
```bash
platformio run
```
You need to edit firebase SSL fingerprint for the first time.
open
`.piolibdeps/FirebaseArduino_ID1259/src/FirebaseHttpClient.h`
and change the fingerprint at the end to [firebaseio.com's fingerprint](https://www.grc.com/fingerprints.htm)
which will most likely be
```
6F D0 9A 52 C0 E9 E4 CD A0 D3 02 A4 B7 A1 92 38 2D CA 2F 26
```
You can now
```bash
platformio run --target upload
```
Initialization gcodes for grbl:
```gcode
$5=0
$10=3
$22=1
$23=255
$24=1000
$25=100
$26=100
$27=0
$100=0.5556
$110=8000
$120=20
$130=359
```
