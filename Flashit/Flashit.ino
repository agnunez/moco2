#include <SoftwareSerial.h>
// configure pinout and ppm limit
#define LED 16        // LED pin
#define PPMLIMIT 1000 // CO2 ppm threshold for warning
#define RX 0          // Receiver pin    // WROOM D3 = GPIO0, D4= GPIO2, D8=GPIO15, D9=GPIO03, D10=GPIO01
#define TX 2          // Transmiter pin  // ESP01   
SoftwareSerial co2(RX, TX);
int ton=100;
int toff=200;

void setup() {
  pinMode(LED, OUTPUT);     
  Serial.begin(9600);
  co2.begin(9600);
  Serial.println("initialized");
}

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
int co2CM1106() {
  static byte cmd[4] = {0x11, 0x01, 0x01, 0xED}; // Commands 0x01 Read ppm, 0x10 open/close ABC, 0x03 Calibrate 
  static byte response[8] = {0};                 // response 0x16, 0x05, 0x01, DF1, DF2, DF3, DF4, CRC.  ppm=DF1*256+DF2
  co2.write(cmd, 4);
  co2.readBytes(response, 8);
  unsigned int chk = (int) response[0];
  unsigned int responseHigh = (unsigned int) response[3];
  unsigned int responseLow = (unsigned int) response[4];
  int crc = 0;
  for (int i = 0; i < 7; i++) crc += response[i];
  crc = 256 - crc%256;
  if (((int) response[0] == 0x16) && ((int)response[7] == crc)) {
    return (256 * responseHigh) + responseLow;
  } else {
    while(co2.available() > 0)  char t = co2.read();  // Clear serial input buffer;
    Serial.println("Serial error!");
    return -1; 
  }
}


// the loop function runs over and over again forever
void loop() {
  int ppm=co2CM1106();
  Serial.println(ppm);
  delay(5000);                       // wait for a second
  ppmflash(ppm);
}
