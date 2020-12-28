#ifndef OLED
#define OLED
#endif
// Libraries required
// https://github.com/ThingPulse/esp8266-oled-ssd1306
// https://github.com/pasko-zh/brzo_i2c
//
#ifdef NODEMCU
#define OLED_I2C_ADD 0x3c
#define OLED_SDA 5
#define OLED_SCL 4
#endif

#ifdef ESP01
#define OLED_I2C_ADD 0x3c
#define OLED_SDA 2
#define OLED_SCL 0
#endif
