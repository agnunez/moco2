# MOCO2 Mobile CO2 warning device

This is a very simple **MO**bile **CO2** detector device, based on ESP8266, as a weareable in a hanger on the belt. For that, main goal is "autonomy" so, saving battery (less components and lower consumption). As everyone carry a mobile phone, it will be used for moco2 command, display and alerts. In order to be affordable for every teacher on schools, we are targeting also low cost, in terms of minimum processing requirement and cheapest sensor with reliability.

![Alt Text](https://github.com/agnunez/moco2/blob/master/images/moco2.gif)

The intention is to receive Telegram warning messages on your celular when you are in an dangerous environment with too high CO2 concentration that could means that aerosols containing COVID19 from other persons breath could be present, so you can open windows or exit from that location.

NOTE: This is an on-going development in two pieces: moco2.ino (telegram bot test), flashit (working sketch with single flashing led implementation currently working with CM1106 and MH-Z19B NDIR CO2 sensors and with optional OLED display).

There is a single flashing led instead of a display to save battery with a simple behaviour:
 1. below 400ppm led is kept off
 1. above 400 until a predefined threshold (PPMLIMIT) the led will flash 'n' times depending of the co2 level 
    1. once above 400
    1. twice over 500
    1. three times over 600
    1. ... etc
    so you can distinguish the level of danger by the number of repeating flash in a burst.
 1. aboth PPMLIMIT the led will keep lighting (red colour led is better, but blue built in is ok)

## Sensors and MCU's supported

As the intention is to be very cheap a CM1106 NDIR sensor is used ($4/unit), but MH-Z19B is also supported ($14/unit) althoutg it will require an extra 3.3v regulator, as the adventage of CM1106 is that provides one internally and has an output pin with 3.3v to power the MCU. 
Taking into account that ESP01 cost $1 and a 16850 battery holder with charger also can be bought below $1, the total project cost could be very affordable ($10). 
We intend to provide one of this to every teacher in the school, so they will be responsible to keep open windows, or air conditioning working at the necessary levels.
The code is very generic and with a very reduced BOM (co2 sensor, mcu, lipo battery with charger circuit). We used ESP01 due to cost, but any ESP or even arduino model could be used. If there is no Wifi capability, only the led warning will be available.

## Hardware connections

There is a 3d printed draft PCB that can be either solder or use wirewrapping, as is my case. There are different connection schemes for each combination of sensor and MCU:

### CM1106 and ESP-01

This is the simplest schema, as CM1106 provides 5v(100mA) to 3.3v conversion allowing to power the ESP-01 directly (10mA current with no WiFi and Light_Sleep). We have tested with WiFi but further long term impact on CM1106 is needed.
The diagram just connect 3.3v among the modules and cross Tx and Rx signals. CM1106 is power from 5v, so any USB cable connector can be used from PC, Mobile Charger or even a Power Pack, like in our case. With a 1000mA single 18650 LiPo battery powerbank, more than 12 h has been tested, enough for carriying a working day.

![Alt Text](https://github.com/agnunez/moco2/blob/master/images/moco2_circuit_diagram.jpg)

## Calibration

There is a simple python program to connect a CM1106 to an USB-Ser adapter on a PC or RPI and calibrate it. The program should be run just after the sensor is switched on. It waits until sensor is heated up (550 reading) and, after that,  when sensor start to provides real measurements, the program asks to input the current CO2 ppm. If is it runned outdoors, you can input 410. If you run it indoors, you need another reference CO2 sensor well calibrated providing room ppm to use that measurement for calibration of CM1106.
After inserting that target reading, it takes several seconds to adjust itself to that ppm concentration. When reading is stable, it can be disconected and attached to your ESP or arduino.
ABC is on, so if is kept connected it will autocalibrate every week to the minimum reading as if were 410, so .. open your windows daily at least.


# To Do's
- [x] Sample flashing code (flashit directory)
- [x] Lower consumption with ESP8266 Light Sleep mode. No pin soldering or reset required
- [ ] Telegram bot. ongoing (partial test moco2.ino)
- [ ] Tutorial. ongoing (this github README)
- [ ] BOM materials and links
- [ ] Software requirements (libraries, etc)
- [x] PCB schema
- [x] 3D printed pcb
- [ ] 3D printed case stl 

## Contributions

This project is part of the [CODOS](https://github.com/miguelangelcasanova/codos) initiative to develop devices for schools to improve the safety of the air quality. There is a very active Telegram channel to discuss the open product development in [Telegram CODOS channel](https://t.me/codos_ventilacion) (mostly in spanish).

This code is actively maintained in [https://github.com/agnunez/moco2](https://github.com/agnunez/moco2) by [@agnuca](http://twitter.com/agnuca) and contributions are welcome!
