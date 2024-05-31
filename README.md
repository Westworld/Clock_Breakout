# Arkanoid/Breakout Clock


Run clock for 10 minutes with 
Arkanoid, then 
Tetris, then 
Space Invaders, then
Picture Clock, then
Pacman, then restart with Arkanoid

Idea for Pacman from
https://www.instructables.com/ESP32-Pacman-Clock/
Idea from project: 
https://hackaday.io/project/169655-arkaclock-retro-arcade-arkanoid-with-clock

The source is written for Cheap Yellow Display, an ESP32 board with 320x240 screen for around $10...
https://github.com/witnessmenow/ESP32-Cheap-Yellow-Display.


Use Platformio to build and upload:

Create a file named "wifisetting.py" one level above the project with the Wifi password, such as
print ('\'-DWIFI_SSID=\"mySSID\"\'')
print ()'\'-DWIFI_PASS=\"xyz\"\'')

(you might also use that for other Wifi pass, keys, email password, etc, to make sure you don't expose it to Github)

In Platformio menu select "Build Filesystem Image", then "Upload filesystem Image", followed by Build and Upload.