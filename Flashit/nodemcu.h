#ifndef NODEMCU
#define NODEMCU
#endif
// ESP8266 WeMos WROOM 2 Oled 16850
// GPIO  Function
// 0 D3 / Button right 
// 1 D10
// 2 D4 / LED blue (LED_BUILTIN)
// 3 D9
// 4 OLED SCL
// 5 OLED SDA
// 12  Button down
// 13  Button up
// 14  Button push
// 15  D8
// 16  LED green
// A0  AD (ADC)
//
// define pins
#define RX 0         // Receiver. D4  GPIO0 
#define TX 2         // Transmiter. D3  GPIO2 
#define LED 16       // Led GREEN  GPI016
#define BTN 14       // Button. GPIO14
