#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include secret.h

// Wifi network station credentials defined in secret.h
#define WIFI_SSID "XXXX"
#define WIFI_PASSWORD "XXXXX"
// Telegram BOT Token (Get from Botfather)
#define BOT_TOKEN "nnnnn:xxxxxxxxxxxxxxxxxxxx"
#define CHAT_ID "xxxxxxxxx" // Chat ID of where you want the message to go (You can use MyIdBot to get the chat ID)

#define DEBUG 0            // print log messages in serial 

X509List cert(TELEGRAM_CERTIFICATE_ROOT);
WiFiClientSecure secured_client;
UniversalTelegramBot bot(BOT_TOKEN, secured_client);
int co2=0;

void handleNewMessages(int numNewMessages){  // review last messajes 
  if(DEBUG)Serial.print("handleNewMessages ");
  if(DEBUG)Serial.println(numNewMessages);
  
  String answer;
  for (int i = 0; i < numNewMessages; i++) {
    telegramMessage &msg = bot.messages[i];
    if(DEBUG)Serial.println("Received " + msg.text);
    if (msg.text == "/help")
      answer = "Use /co2 for reading sensor\n/cal to calibrate\n/bat battery status\n/set for setting alarm level\n/help for this explanation\n";
    else if (msg.text == "/co2"){
      answer = "Current CO2: *" + String(co2) + "* ppm";
      co2+=1;
    }
    else if (msg.text == "/cal")
      answer = "Sent new calibration";
    else if (msg.text == "/bat")
      answer = "Battery ok!";
    else if (msg.text == "/set")
      answer = "Setting new alarm level!";
    else
      answer = "Command error. Please use /help";
    bot.sendMessage(CHAT_ID, answer, "Markdown");
  }
}

void bot_setup() {
  const String commands = F("["
                            "{\"command\":\"co2\", \"description\":\"CO2 sensor reading un ppm\"},"
                            "{\"command\":\"cal\",\"description\":\"Calibrate level\"}," 
                            "{\"command\":\"bat\",\"description\":\"Show battery level\"}," 
                            "{\"command\":\"set\",\"description\":\"Set new alarm level\"}," 
                            "{\"command\":\"help\",\"description\":\"Get usage help\"}" // no comma on last command
                            "]");
  ///secured_client.setInsecure();  /// uncoment if secured connection does not work. @agnuca
  if(DEBUG)Serial.println("setclient ok");

  //bot.setMyCommands(commands);
  //bot.sendMessage(CHAT_ID, "Hola amigo!", "Markdown");
}

void setup() {
  if(DEBUG)Serial.begin(115200);
  if(DEBUG)Serial.println();

  // attempt to connect to Wifi network:
  secured_client.setTrustAnchors(&cert); // Add root certificate for api.telegram.org
  if(DEBUG)Serial.print("Connecting to Wifi SSID ");
  if(DEBUG)Serial.print(WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    if(DEBUG)Serial.print(".");
    delay(500);
  }
  if(DEBUG)Serial.print("\nWiFi connected. IP address: ");
  if(DEBUG)Serial.println(WiFi.localIP());
  bot_setup();
  if(DEBUG)Serial.println("bot_setup ok");
}

void loop() {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    while (numNewMessages) {
      if(DEBUG)Serial.println("got response");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
    delay(1000);
}
