#define OLED 1   // uncomment to use oled display
//#define DEBUG 1  // uncomment to debug through serial usb console
#include <SoftwareSerial.h>
#ifdef OLED
#include <SSD1306Brzo.h> // Include OLED Library
#endif

// configure pinout and ppm limit
#define LED 16        // LED pin
#define PPMLIMIT 1000 // CO2 ppm threshold for warning
#define RX 0          // Receiver pin    // WROOM D3 = GPIO0, D4= GPIO2, D8=GPIO15, D9=GPIO03, D10=GPIO01
#define TX 2          // Transmiter pin  // ESP01   
#define CM1106        // uncomment the co2 sensor used
//#define MHZ19B      //  "             "
SoftwareSerial co2(RX, TX);  // leave free standar uart for pc usb debug
#ifdef OLED
SSD1306Brzo display(0x3c, 5, 4); // Initialize OLED display
#endif

int ton=100;          // microsec to light the led while flashing
int toff=200;         // microsec to dimm the led while flashsing

void setup() {
  pinMode(LED, OUTPUT);
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
void drawText(int ppm) { // Fuction to draw the text
display.flipScreenVertically();
display.setTextAlignment(TEXT_ALIGN_LEFT);
display.setFont(ArialMT_Plain_16);
display.drawString(0, 0, "CO2");
display.setFont(ArialMT_Plain_24);
display.drawString(40, 20, String(ppm)); // Print button press
display.setFont(ArialMT_Plain_16);
display.drawString(80, 46, "ppm" ); // Print value of analog input
}
#endif

void flash(int n){          // flash n times
  if (n<0 || n>10) return;  // safety
  for(int i=0;i<n;i++){
    digitalWrite(LED, LOW);
    delay(ton);
    digitalWrite(LED, HIGH);
    delay(toff); 
  }
}

void ppmflash(int ppm){
  if (ppm < 500) {           // LED off for safe ppm
    digitalWrite(LED,HIGH);
  } else if (ppm>PPMLIMIT) { // LED stay ON when over the limit (red color led preferred)
    digitalWrite(LED,LOW); 
  } else {
    flash(int(ppm/100-4));   // LED flashes several times every hundred ( >500 once, >600 twice, >700 three times....)
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
  display.clear(); // Clear OLED display
  drawText(ppm); // Draw the text
  display.display(); // Write the buffer to the display
#endif
#ifdef DEBUG
  Serial.println(ppm);
#endif
  delay(5000);                       // wait for a second
  ppmflash(ppm);
}
