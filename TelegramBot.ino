/*******************************************************************
*  Based on an example of bot that echos back any messages received*
*  written by Giacarlo Bacchio (Gianbacchio on Github)             *
*  adapted by Brian Lough                                          *
*                                                                  *
*  Re-addapted by David Scholten (palanthas on Github)             *
*  This is a work in progress...                                   *
*******************************************************************/
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>

// Initialize Wifi connection to the router
char ssid[] = "SSID";     // your network SSID (name)
char password[] = "wifiPassword"; // your network key

// Initialize Telegram BOT
#define BOTtoken "xxxxxxxxx_xxxxxxxxxxxxxxxxxxxxxxxx"  // your Bot Token (Get from Botfather)

WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

int Bot_mtbs = 1000; //mean time between scan messages
long Bot_lasttime;   //last time messages' scan has been done

String chat_id = "447498028";   

#define LED_PIN 2

void setup() {
  Serial.begin(115200);
  myBlinkFunction(200, 3); // Blink the light. First arg is speed of the blink in millisecs and second arg is number of blinks

  // Attempt to connect to Wifi network:
  Serial.print("Connecting Wifi: ");
  Serial.println(ssid);

  // Set WiFi to station mode and disconnect from an AP if it was previously connected
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    myBlinkFunction(100, 1);
    delay(300);
  }

  Serial.println("");
  digitalWrite(LED_PIN, HIGH);
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  bot.sendMessage(chat_id, "ESP32 bot is back online!", "Markdown");
}

void loop() {
  if (millis() > Bot_lasttime + Bot_mtbs)  {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    while(numNewMessages) {
      Serial.print("Got response: ");
//      digitalWrite(LED_PIN, LOW);
      myBlinkFunction(200, 2);
      
      for (int i=0; i<numNewMessages; i++) {
        Serial.println(bot.messages[i].text);
        String chat_id = String(bot.messages[i].chat_id);
        String fullMsg = "Your comment was: " + bot.messages[i].text;
        bot.sendMessage(chat_id, fullMsg, "Markdown"); // String chat_id, String text or VAR or bot.messages[i].text, String parse_mode = "" ("Markdown" in this case)
        digitalWrite(LED_PIN, HIGH);
      }
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }

    Bot_lasttime = millis();
  }
}

///  FUNCTIONS  ///

int c = 0;

void myBlinkFunction(int blinkSpeed, int blinkCount){
  for (c = blinkCount; c > 0;){
    pinMode(LED_PIN, OUTPUT);
      digitalWrite(LED_PIN, HIGH);
      delay(blinkSpeed);
      digitalWrite(LED_PIN, LOW);
      delay(blinkSpeed);
    c--;
  }
}
