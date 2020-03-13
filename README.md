# Arkanoid/Breakout Clock

Idea from project: 
https://hackaday.io/project/169655-arkaclock-retro-arcade-arkanoid-with-clock


The source is written for two different versions.
One with 2.4" 8bit parallel TFT 320x240 and ESP32 (with enough pins for 8bit parallel)
The other with 4" SDI TFT 480x320 and ESP8266/Wemos.


In platformio.ini select the version to use.


Video 4" version:
https://youtu.be/bO-brxepqRM

Video 2.4" version:
https://youtu.be/p1KVjlZ9Ves


Create a file named "wifisetting.py" one level above the project with the Wifi password, such as
print '\'-DWIFI_PASS=\"xyz\"\''
to give second Wifi-SSID and Pass, just add more lines.

Note:
I used lazy concept to switch between both versions.
The ini file sets Target as 32 or 8266, which sets the chip AND TFT type (library, size, etc)
If you use SDI for 2.4 inch, you need to adapt that yourself.

For the 2.4" inch version I used an UNO prototype PCB and some wires.
For the 4" inch version I created a PCB and used a simple CNC mill.
The PCP etch/drill file and Eagle files are in the Eagle folder.


Case is 3D printed, download STL or modify:
https://www.tinkercad.com/things/hlhGvt64I7I-tft-24-and-40-case

