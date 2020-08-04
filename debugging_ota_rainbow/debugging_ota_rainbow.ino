
/********************************** INCLUDES **********************************/
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <FastLED.h>
#include <ArduinoOTA.h>
#include "led_effect_increment.h"

/***************************** GENERIC VARIABLES ******************************/
/************************* MICROCONTROLLER SPECIFICS **************************/
// TODO: change variable name of mqtt topic to something more
// universal across all files. An idea for this is "lightStateTopic"
// as a variable name

// MQTT specific
// work_bench
// desk
// table
// const char* lightSetTopic = "home/new/table/set";
// const char* mqttTopicResponse = "home/new/table";
const char* lightSetTopic = "home/new/debug/set";
const char* mqttTopicResponse = "home/new/debug";
const char* mqttServer = "192.168.0.14";
const int mqttPort = 1883;
// mqtt user and pass can be shared (I think)
const char* mqttUsername = "debug_strand";
const char* mqttPassword = "debug_strand_pass";

const char* on_cmd = "on";
const char* off_cmd = "off";
const char* effect = "solid";
String effectString = "solid";
String oldeffectString = "solid";

// OTA Username and pass
#define SENSORNAME "debug_strand"
#define OTApassword "truc%s1"
int OTAport = 8266;

// Wifi network name and password
const char* ssid = "BHNTC8715D152A";
const char* password =  "da22f874";

// declare object of class wificlient, also pubsub
WiFiClient espClient;
PubSubClient client(espClient);

// For JSON
const int BUFFER_SIZE = JSON_OBJECT_SIZE(10);
// TODO: both of these are probably not necessary, one of bruh and one
// is what I originaly did.
#define MQTT_MAX_PACKET_SIZE 512
const int capacity = 256;

// FastLED Definitions
#define NUM_LEDS 76    // TODO: numleds changes from deivce to device
#define DATA_PIN 5
#define CHIPSET WS2812B
#define COLOR_ORDER GRB
byte brightness = 255;
byte red = 0;
byte green = 0;
byte blue = 0;
byte realRed = 0;
byte realGreen = 0;
byte realBlue = 0;
CRGB leds[NUM_LEDS];
// struct CRGB leds[NUM_LEDS];  // bruh uses this

// Globals for fade/flash  // from bruh, I don't know if I need these
// TODO: If this is necessary, add the rest of the Bruh stuff. If not,
// delete it
bool stateOn = false;
int transitionTime = 0;


/**************************** HOUSEKEEPING SHARED *****************************/
/*** my specific stuff for led rainbow ***/
float rgbArray[3] {255, 0, 0};
// TODO: I might have to change variables in the function name to
// prevent shit from going wrong.  It's probably better if I make it a
// header file and just call the function from somehwere else.
int rainbowCycleLEDWrite;

/**************************** GLOBALS FOR EFFECTS *****************************/
// rainbow
uint8_t thishue = 0;
uint8_t deltahue = 10;

// TODO: add the other effects (candycane, noise, twinke...) if I want
// them later


int successfulReturnFlag;
// void callback(char* topic, byte* payload, unsigned int length)
char * callbackTopic;
byte * callbackPayload;
unsigned int callbackLength;
StaticJsonDocument<capacity> doc;

/*********************** SETUP (CONNECT TO EVERYTHING) ************************/
void setup() {
  Serial.begin(115200);

  // TODO: break this up
  // connectToWifiAndMQTT();
  setupWifi();
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
  
  client.subscribe(lightSetTopic);
  FastLED.addLeds<CHIPSET, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS);

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


void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");


	/* int * successfulReturnFlag; */
	/* // void callback(char* topic, byte* payload, unsigned int length) */
	/* char * callbackTopic; */
	/* byte * callbackPayload; */
	/* unsigned int callbackLength; */
	
	successfulReturnFlag = 0;

	// StaticJsonDocument<capacity> doc;
  DeserializationError err = deserializeJson(doc, payload, length);
  if (err){
    Serial.print("ERROR IN CALLBACK: ");
    Serial.println(err.c_str());
    return;
  } else {
		// Sucessful return

		callbackTopic = topic;
		callbackPayload = payload;
		callbackLength = length;
		
		successfulReturnFlag = 1;
		return;
	}

//  if(!processJson(payload, length)) {
//    return;
//  }
//  if(stateOn) {
//    realRed = map(red, 0, 255, 0, brightness);
//    realGreen = map(green, 0, 255, 0, brightness);
//    realBlue = map(blue, 0, 255, 0, brightness);
//  } else {
//    realRed = 0;
//    realGreen = 0;
//    realBlue = 0;
//  }
//  Serial.println(effect);
//  // startFade = true;
//  // inFade = false;     // kill current fade
//
//  sendState();
}


// bool processJson(byte* payload, unsigned int length) {
bool processJson() {
  // StaticJsonDocument<capacity> doc;
  // DeserializationError err = deserializeJson(doc, callbackPayload, callbackLength);
  // if (err){
    // Serial.print("ERROR IN PROCESS JSON: ");
    // Serial.println(err.c_str());
    // return false;
  // }
  
  JsonObject root = doc.as<JsonObject>();
  
  // Check State
  const char* state = root["state"];
  if (state) {
    Serial.println(state);
    // If it gets here, a state was sent
    // Check if it's on or off
    if (strcmp(root["state"], on_cmd) == 0){
      stateOn = true;
    }
    else if (strcmp(root["state"], off_cmd) == 0){
      stateOn = false;
    }
  }

  // I might put this in callback and not worry about anything
  /**************************** PROCESS JSON SECTION ****************************/
  if(root.containsKey("state")) {
    if(strcmp(root["state"], on_cmd) == 0) {
      stateOn = true;
    } else if(strcmp(root["state"], off_cmd) == 0) {
      stateOn = false;
    }
  }
  if(root.containsKey("color")) {
    red = root["color"]["r"];
    green = root["color"]["g"];
    blue = root["color"]["b"];
  }

  if(root.containsKey("brightness")) {
    brightness = root["brightness"];
  }
  if(root.containsKey("effect")) {
    effect = root["effect"];
    effectString = effect;
  }
  if(root.containsKey("transition")) {
    transitionTime = root["transition"];
  } else if(effectString == "solid") {
    transitionTime = 0;
  }
}


void sendState() {
  /************************************ SEND ************************************/
  // Now that I've checked for a state I can send a response
  // This also should be in it's own function
  // In bruh, start send state is on line 418
  // It's its own function
  StaticJsonDocument<capacity> returnDoc;
  returnDoc["state"] = (stateOn) ? on_cmd : off_cmd;

  JsonObject color = returnDoc.createNestedObject("color");
  color["r"] = red;
  color["g"] = green;
  color["b"] = blue;
  
  returnDoc["brightness"] = brightness;
  returnDoc["effect"] = effectString.c_str();

  Serial.print("real red: ");
  Serial.println(realRed);
  Serial.print("real green: ");
  Serial.println(realGreen);
  Serial.print("real blue: ");
  Serial.println(realBlue);
  
  char buffer[512];
  size_t n = serializeJson(returnDoc, buffer);
  Serial.println("under computer desk topic received");
  client.publish(mqttTopicResponse, buffer, n); // lightStateTopic
}

void reconnect() {
  // Bruh's reconnect, I copied it exactly
  // loop until reconnected
  while(!client.connected()) {
    Serial.print("Attempting MQTT connection... ");
    // Attempt to connect
    if(client.connect(SENSORNAME, mqttUsername, mqttPassword)) {
      Serial.println("connected");
      delay(5000);
      Serial.print("subscribing to: ");
      Serial.println(lightSetTopic);
      client.subscribe(lightSetTopic);
//      setColor(0, 0, 0);
//      sendState();
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // wait 5 seconds
      delay(5000);
    }
  }
}


void setColor(int inR, int inG, int inB) {
  for(int i = 0; i < NUM_LEDS; i++) {
    leds[i].red = inR;
    leds[i].green = inG;
    leds[i].blue = inB;
  }
  FastLED.show();
  // Bruh follows this with printing out color information. I don't
  // care enough to do that for now.
}

void loop() {
  if(!client.connected()) {
    reconnect();
  }
  if(WiFi.status() != WL_CONNECTED) {
    delay(1);
    Serial.print("WIFI Disconnected. Attempting reconnection.");
    setupWifi();
  }
  
  client.loop();
  ArduinoOTA.handle();

	if(successfulReturnFlag) {
		processJson();
	}

  // EFFECTS INFO HERE

  /*** EFFECT RAINBOW ***/
  if(effectString == "rainbow") {
    // TODO: Add brightness to this
    if(!properRainbowIncrement(rgbArray, NUM_LEDS)) {
      setColor((int)rgbArray[0], (int)rgbArray[1], (int)rgbArray[2]);
    } else {
      Serial.print("RAINBOW ERROR 1");
    }
    delay(0.1);
  }

  /*** EFFECT RAINBOW CYCLE ***/
  if(effectString == "rainbow cycle") {
    // TODO: Add brightness to this
    // Consider other ways to get to starting state

    // Logic through this
    // If we go out of the bounds of LEDs, go back to zero. Obvious
    if(rainbowCycleLEDWrite >= 300) {
      rainbowCycleLEDWrite = 0;
      if(properRainbowIncrement(rgbArray, NUM_LEDS)) {
        Serial.print("RAINBOW CYCLE ERROR 2");
        setColor(255, 0, 0);
        sendState();
      }
    }

    // This is where the LED is actually written. One led per loop
    if(!properRainbowIncrement(rgbArray, NUM_LEDS)) {
      leds[rainbowCycleLEDWrite].red = (int)rgbArray[0];
      leds[rainbowCycleLEDWrite].green = (int)rgbArray[1];
      leds[rainbowCycleLEDWrite].blue = (int)rgbArray[2];
    } else {
      Serial.print("RAINBOW CYCLE ERROR 1");
      setColor(0, 255, 0);
      sendState();
    }
    delay(0.1);
    rainbowCycleLEDWrite++;
    FastLED.show();
  }

  /*** EFFECT SOLID ***/
  if(effectString == "solid") {
    // brightness adjust
    // brightness adjust might need to be it's own function
    // Serial.println("state on true, adjusting brightness");
    realRed = map(red, 0, 255, 0, brightness);
    realGreen = map(green, 0, 255, 0, brightness);
    realBlue = map(blue, 0, 255, 0, brightness);

    setColor(realRed, realGreen, realBlue);
  }
}
