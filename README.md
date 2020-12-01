# MOCO2 Mobile CO2 warning device

This is a very simple MObile CO2 detector device, based on ESP8266, to be carried in a hanger on the belt. 
The intention is to receive Telegram warning messages on your celular when you are in an dangerous environment with too high
CO2 concentration that could mean COVID19 aerosols from other persons breath, so you can open windows or exit from the room.

There is a single flashing led instead of a display to save battery with a simple behaviour:
 1. below 500ppm led is kept off
 1. above 500 until a predefined threshold (PPMLIMIT) the led will flash 'n' times depending of the co2 level 
    1. once above 500
    1. twice over 600
    1. three times over 700
    1. ... etc
    so you candistinguish the level of danger.
 1. aboth PPMLIMIT the led will keep lighting (red colour led is better)

## Sensors and MCU's supported

As the intetion is to be very cheap, a CM1106 NDIR sensor is used ($4/unit), but MH-Z19B is also supported ($14/unit).
Taking into account that ESP01 cost $1 and a 16850 battery holder with charger also can be bought below $1, the total
project cost could be very affordable ($10). We intend to provide one of this to every teacher in the school, so
they will be responsible to keep open windows, or air conditioning working at the necessary levels.
The code is very generic and with a very reduced BOM (co2 sensor, mcu, lipo bat with charger). We used ESP01 due to 
cost, but any ESP or even arduino model could be used. If there is no Wifi capability, only the led warning will be
available.

## Contributions

The project is part of the [CODOS](https://github.com/miguelangelcasanova/codos) to develop devices for schools to improve the safety of the air quality.
There is a very actve Telegram channel to discuss this initiative in [Telegram CODOS channel](https://t.me/codos_ventilacion) (mostly in spanish).

This code is actively maintained in [https://github.com/agnunez/moco2](https://github.com/agnunez/moco2) by [@agnuca](http://twitter.com/agnuca) and contributions are welcome!
