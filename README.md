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

# To Do's
- [x] Sample flashing code (flashit directory)
- [x] Lower consumption with ESP8266 Light Sleep mode. No pin soldering or reset required
- [ ] Telegram bot. ongoing (partial test moco2.ino)
- [ ] Tutorial. ongoing (this github README)
- [ ] BOM materials and links
- [ ] Software requirements (libraries, etc)
- [ ] PCB schema
- [x] 3D printed pcb
- [ ] 3D printed case stl 

## Contributions

This project is part of the [CODOS](https://github.com/miguelangelcasanova/codos) initiative to develop devices for schools to improve the safety of the air quality. There is a very active Telegram channel to discuss the open product development in [Telegram CODOS channel](https://t.me/codos_ventilacion) (mostly in spanish).

This code is actively maintained in [https://github.com/agnunez/moco2](https://github.com/agnunez/moco2) by [@agnuca](http://twitter.com/agnuca) and contributions are welcome!
