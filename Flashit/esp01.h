#ifndef ESP01
#define ESP01 
#endif

#ifdef NODEMCU
#error Cannot defined ESP01 AND NODEMCU
#endif

//  Header Pinout
//  _________
//  |       |
//  |8 6 4 2|
//  |7 5 3 1|
//  |[ ] [ ]|
//  |    __||
//  |   |||||
//  |_______|
// 1 Ground - Connected to the ground of the circuit
// 2 TX GPIO01 Connected to Rx pin of programmer to upload program. Can act as a General purpose Input/output pin when not used as TX
// 3 GPIO02 General purpose Input/output pin
// 4 CH_EN Chip Enable – Active high
// 5 GPIO00 Flash. General purpose Input/output.  If hold LOW while switch on takes module into serial programming
// 6 Reset the ESP8266
// 7 RX GPIO03 General purpose Input/output pin. Can act as a General purpose Input/output pin when not used as RX
// 8 Vcc +3.3v. Do not connect above +3.3v!!

#define RX 3          // Receiver pin   D3 or GPIO0 
#define TX 1          // Transmiter pin D4 or GPIO2
#define LED 2        // Led to flash GPIO16
#define BTN 0        // Mini joy push button    
