// Choose a CO2 NDIR sensor by uncommenting just one
#define CM1106         // uncomment on the supported CO2 sensors
//#define MHZ19B       //  "         "            "

// Choose a MCU by uncommenting just one.  Pins are defined in .h files and can be modified there
#define ESP01          // uncomment used MCU 
//#define NODEMCU      //  "         "
//#define WIFI        // uncomment if we need WiFi but prevent the usage of sleepdelay low power consumption

// Choose options at wish.
#define OLED         // uncomment to use oled display
//#define DEBUG        // uncomment to debug through serial usb console (Warning! with ESP01 you need differeuznt pins for standard & optional Serials)

const int PPMLIMIT=1000; // CO2 ppm threshold for warning
const int TON=100;       // Time to keep led ON during flashing
const int TOFF=200;      // Time to keep led OFF during flashing
const int DELAY=10000;   // Delay among measures
const int BIAS=3;        // units to reduce no. of flashes from hundreds in ppm (i.e. BIAS 3 => 400..1 flash, 500..2 flashes ) 

// 
// Configure your options above this and in include.h files
// 

#ifdef NODEMCU              // include NodeMCU options
#include "nodemcu.h"
#endif

#ifdef ESP01
#include "esp01.h"          // include ESP01 options
#endif

#ifdef OLED                 // includes OLED options and li4brary
#include "oled.h"
#include <brzo_i2c.h>
#include <SSD1306Brzo.h> 
#include "font.h"       
#endif

#ifndef WIFI
void WiFiOff();
void sleepdelay(int t);
#endif

#include <SoftwareSerial.h>  // Mandatory for CO2 sensor comms. It's used to avoid collision with standard Serial & devote it only for sensor comm's.
SoftwareSerial co2(RX, TX);  // leave free standard uart for pc usb debug

#ifdef OLED
SSD1306Brzo display(OLED_I2C_ADD, OLED_SDA, OLED_SCL);  // Initialize OLED display
#endif

void setup() {
  pinMode(LED, OUTPUT);
  delay(1000);
  digitalWrite(LED, HIGH);
#ifdef DEBUG
  Serial.begin(9600);
#endif
  co2.begin(9600);
#ifdef OLED
display.init(); // Initialise the display
#endif
#ifdef DEBUG
  Serial.println("initialized");
#endif
}

#ifdef OLED
void drawText(int ppm) { // Fuction to draw the text in oled display
  String sppm = String(ppm,DEC);
  display.init(); // Initialise the display
  display.clear();    // Clear OLED display
  display.flipScreenVertically();
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.setFont(ArialMT_Plain_16);
  display.drawString(15, 0, "CO2");
  display.setFont(Dialog_plain_26);
  display.drawString(64, 20, sppm);
  display.setFont(ArialMT_Plain_16);
  display.drawString(110, 46  , "ppm" ); // Print value of analog input
  display.display();  // Write the buffer to the display
}
#endif

void mydelay(int t){
#ifndef WIFI
  sleepdelay(t);
#else
  delay(t);
#endif  
}

void flash(int n){          // flash n times
  if (n<0) return;  // safety
  for(int i=0;i<n;i++){
    digitalWrite(LED, LOW);
    mydelay(TON);
    digitalWrite(LED, HIGH);
    mydelay(TOFF); 
  }
}

void ppmflash(int ppm){
  if (ppm < 400) {           // LED off for safe ppm
    digitalWrite(LED,HIGH);
    mydelay(TON); 
  } else if (ppm>PPMLIMIT) { // LED stay ON when over the limit (red color led preferred)
    digitalWrite(LED,LOW); 
    mydelay(TON); 
  } else {
    flash(int(ppm/100)-BIAS);   // LED flashes several times every hundred ( >400 once, >500 twice, >600 three times....) until PPMLIMIT that stay ON
  }
}

// SoftwareSerial co2(RX, TX);    // set this software serial interface on RX and TX whished gpios.
#ifdef CM1106
int co2CM1106() {
  static byte cmd[4] = {0x11, 0x01, 0x01, 0xED}; // Commands 0x01 Read ppm, 0x10 open/close ABC, 0x03 Calibrate 
  static byte response[8] = {0};                 // response 0x16, 0x05, 0x01, DF1, DF2, DF3, DF4, CRC.  ppm=DF1*256+DF2
  co2.write(cmd, 4);
  co2.readBytes(response, 8);
  int crc = 0;
  for (int i = 0; i < 7; i++) crc += response[i];
  crc = 256 - crc%256;
  if (((int) response[0] == 0x16) && ((int)response[7] == crc)) {
    unsigned int responseHigh = (unsigned int) response[3];
    unsigned int responseLow = (unsigned int) response[4];
    return (256 * responseHigh) + responseLow;
  } else {
    while(co2.available() > 0)  char t = co2.read();  // Clear serial input buffer;
    return -1; 
  }
}
#endif
#ifdef MHZ19B
int co2MHZ19B(){
  static byte cmd[9] = {0xFF, 0x01, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00, 0x79};
  static byte response[9]; 
  co2.write(cmd, 9); 
  co2.readBytes(response, 9);
  int crc = 0;
  for (int i = 1; i < 8; i++) crc += response[i];
  crc = 255 - crc + 1;
  if ((response[1] == 0x86) && ((int)response[8] == crc)) {
    unsigned int responseHigh = (unsigned int)response[2];
    unsigned int responseLow = (unsigned int)response[3];
    return (256 * responseHigh) + responseLow;
  } else {
    while(co2.available() > 0)  char t = co2.read();  // Clear serial input buffer;
    return -1; 
  }
}
#endif

// the loop function runs over and over again forever
void loop() {
#ifdef CM1106
  int ppm=co2CM1106();
#endif
#ifdef MHZ19B
  int ppm=co2MHZ19B();
#endif
#ifdef OLED
  drawText(ppm);      // Draw the text
#endif
#ifdef DEBUG
  Serial.println(ppm);
#endif
  ppmflash(ppm);
  mydelay(DELAY);       // wait for delay
}
