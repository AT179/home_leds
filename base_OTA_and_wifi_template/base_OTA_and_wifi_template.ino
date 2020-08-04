#include <ESP8266WiFi.h>
#include <ArduinoOTA.h>

// OTA Username and pass
#define SENSORNAME "behind_desk"
#define OTApassword "truc%s1"
int OTAport = 8266;

// Wifi network name and password
const char* ssid = "BHNTC8715D152A";
const char* password =  "da22f874";

// declare object of class wificlient
WiFiClient espClient;

void setup() {
  Serial.begin(115200);
  setupWifi();
  setupOTA();

}

void setupWifi() {
  WiFi.begin(ssid, password);
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address ");
  Serial.println(WiFi.localIP());
}

void setupOTA() {
  //OTA SETUP
  ArduinoOTA.setPort(OTAport);
  ArduinoOTA.setHostname(SENSORNAME);  // Hostname defaults to esp8266-[ChipID]

  // No authentication by default
  ArduinoOTA.setPassword((const char *)OTApassword);
  ArduinoOTA.onStart([]() {
    Serial.println("OTA Starting");
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nOTA End");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });

  ArduinoOTA.begin();

  Serial.println("OTA begin");
  Serial.println("Ready");
}

void loop() {
  if(WiFi.status() != WL_CONNECTED) {
    delay(1);
    Serial.print("WIFI Disconnected. Attempting reconnection.");
    setupWifi();
  }
  
    ArduinoOTA.handle();
}

//#include <Adafruit_NeoPixel.h>
//
//#define NUM_LEDS 50    // TODO: numleds changes from deivce to device
//#define DATA_PIN 5
//#define CHIPSET WS2812B
//#define COLOR_ORDER GRB
//
//
//// Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, DATA_PIN, NEO_GRBW + NEO_KHZ800);
//Adafruit_NeoPixel strip(NUM_LEDS, DATA_PIN, NEO_GRBW + NEO_KHZ800);
//
//strip.begin();
//strip.show(); // Initialize all pixels to 'off'
//
//// strip.setPixelColor(n, red, green, blue, white);
//for(int i = 0; i<NUM_LEDS; i++) {
//  strip.setPixelColor(i, 255, 0, 0, 0);
//}
//
//strip.show();
//
//void setup() {
//  strip.begin();
//  strip.show(); // Initialize all pixels to 'off'
//
//  // strip.setPixelColor(n, red, green, blue, white);
//  for(int i = 0; i<NUM_LEDS; i++) {
//    strip.setPixelColor(i, 255, 0, 0, 0);
//  }
//}
//
//void loop() {
//  // put your main code here, to run repeatedly:
//
//  Serial.print("red");
//  for(int i = 0; i<NUM_LEDS; i++) {
//    strip.setPixelColor(i, 255, 0, 0, 0);
//  }
//  strip.show();
//  delay(5000);
//
//  Serial.print("green");
//  for(int i = 0; i<NUM_LEDS; i++) {
//    strip.setPixelColor(i, 0, 255, 0, 0);
//  }
//  strip.show();
//  delay(5000);
//
//  Serial.print("blue");
//  for(int i = 0; i<NUM_LEDS; i++) {
//    strip.setPixelColor(i, 0, 0, 255, 0);
//  }
//  strip.show();
//  delay(5000);
//
//  Serial.print("white");
//  for(int i = 0; i<NUM_LEDS; i++) {
//    strip.setPixelColor(i, 0, 0, 0, 255);
//  }
//  strip.show();
//  delay(5000);
//
//}
