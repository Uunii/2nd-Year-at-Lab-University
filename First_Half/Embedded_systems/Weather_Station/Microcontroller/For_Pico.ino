                                      //libraries
#include <SPI.h>                      //for portocol communication
#include <Wire.h>                     //for connecting 2 sensor both in i2c address
#include <Adafruit_GFX.h>             //for lcd screen
#include <Adafruit_ST7735.h>          //for lcd screen
#include <Adafruit_SGP30.h>           //for gas sensor
#include "HP20x_dev.h"                //for barometer
#include "DHT.h"                      //for temp & hum sensor
#include <WiFi.h>                     //wifi
#include <PubSubClient.h>             //for mqqt to HA
#include <HTTPClient.h>               //for backend

#define TFT_CS   17
#define TFT_DC    3
#define TFT_RST   6
#define TFT_BL    2       //pins for lcd

Adafruit_ST7735 tft(TFT_CS, TFT_DC, TFT_RST);

#define BTN_UP     10
#define BTN_RIGHT  11
#define BTN_DOWN   14
#define BTN_LEFT   15    //pins for buttons to control menu

#define DHTPIN 0
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);  //temp& hum

Adafruit_SGP30 sgp;    //gas sensor
HP20x_dev hp;          //barometer

const int PIN_WIND_ADC = 26;    //pin for analog pin to measure voltage from dc motor
#define MAX_WIND_MS   15.0f     //making 3.3v which is max voltage to 15km/s (just brief guessing)

const char* WIFI_SSID = "*******";  //wifi name
const char* WIFI_PASS = "*******";  //wifi password

const char* MQTT_SERVER = "***.***.***.**"; //ip adress of HA
const int   MQTT_PORT   = 1883;  //HA port
const char* MQTT_USER   = "*****";  //user name on HA for mqtt broker
const char* MQTT_PASS   = "*****";  //password

WiFiClient net; 
PubSubClient mqtt(net);

const char* BACKEND_URL = "http://***.**.***.***:*****/api/upload";  //ip addross of pc running the backend

bool allowConnection = false;  //dont connect to wifi at the start to pervent screen freeze
bool rightWasPressed = false;  //only connect when right button is pressed so start with false

unsigned long lastRead = 0;    //set up for when the sensors read or sent data to
unsigned long lastSend = 0;

float g_temp = 0;
float g_hum  = 0;
float g_co2  = 0;
float g_voc  = 0;
float g_pres = 0;
float g_alt  = 0;
float g_wind = 0;    //for sensor datas

int screen = 0;      //start from main menu
int menuIndex = 0;

const char* menuItems[] = {
  "Temp & Humidity",
  "Air Quality",
  "Barometer",
  "Wind Speed",
  "Connection"        //list of items in main menu
};
const int MENU_COUNT = 5;    //coount of items on the main menu to control selector

bool pressed(int pin) {
  return digitalRead(pin) == LOW;  //for buttons
}

void connectWiFi() {
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(200);                              //function to connect wifi
  }
}

void connectMQTT() {
  mqtt.setServer(MQTT_SERVER, MQTT_PORT);
  while (!mqtt.connected()) {
    mqtt.connect("WeatherStation1", MQTT_USER, MQTT_PASS);
    delay(300);                                                //funtion to connect to MQTT broker
  }
}

void publishWeather(float t, float h, float co2, float voc, float p, float a, float w) {
  char json[200];
  sprintf(json,
    "{\"temp\":%.1f,\"hum\":%.1f,\"co2\":%.0f,\"voc\":%.0f,\"pres\":%.1f,\"alt\":%.1f,\"wind\":%.1f}",
    t, h, co2, voc, p, a, w
  );                               //for publishing data to MQTT as json format

  mqtt.publish("weather/Uunii", json); 
}

void sendToBackend() {
  if (WiFi.status() != WL_CONNECTED) return;  

  HTTPClient http;
  http.begin(BACKEND_URL);
  http.addHeader("Content-Type", "application/json");

  String json = "{";
  json += "\"temperature\":" + String(g_temp) + ",";
  json += "\"humidity\":" + String(g_hum) + ",";
  json += "\"co2\":" + String(g_co2) + ",";
  json += "\"voc\":" + String(g_voc) + ",";
  json += "\"pressure\":" + String(g_pres) + ",";
  json += "\"altitude\":" + String(g_alt) + ",";
  json += "\"wind_speed\":" + String(g_wind);
  json += "}";

  int code = http.POST(json);
  Serial.print("HTTP Response: ");
  Serial.println(code);

  http.end();
}

void updateAllSensors() {
  g_temp = dht.readTemperature();
  g_hum  = dht.readHumidity();

  sgp.IAQmeasure();
  g_co2 = sgp.eCO2;
  g_voc = sgp.TVOC;

  g_pres = hp.ReadPressure() / 100.0;
  g_alt  = hp.ReadAltitude() / 100.0;

  int raw = analogRead(PIN_WIND_ADC);
  float volts = (raw * 3.3) / 4095.0;
  if (volts < 0.05) volts = 0;
  float mps = (volts / 3.0) * MAX_WIND_MS;
  g_wind = mps * 3.6;                           //update all sensors at same to to upload all readings at same time
}

void drawMenu() {
  tft.fillScreen(ST77XX_BLACK);                //set the background of screen black

  tft.setTextSize(2);                          //text size
  tft.setTextColor(ST77XX_YELLOW);             //color of text
  tft.setCursor(26, 4);                        //setting cursor position where to start next textx
  tft.println("MENU"); 

  tft.drawLine(0, 25, 128, 25, ST77XX_CYAN);   //drawing line on screen (everything is same after this)

  tft.setTextSize(1);
  for (int i = 0; i < MENU_COUNT; i++) {
    int y = 38 + i * 25;
    tft.setCursor(8, y);

    if (i == menuIndex) {
      tft.fillRect(1, y - 8, 120, 24, ST77XX_YELLOW);
      tft.setTextColor(ST77XX_BLACK);
    } else {
      tft.setTextColor(ST77XX_WHITE);
    }
    tft.print(menuItems[i]);
  }
  delay(150);
}

void showTempHum() {
  tft.fillScreen(ST77XX_BLACK);
  tft.setTextColor(ST77XX_YELLOW);
  tft.setCursor(10, 10);
  tft.println("TEMP & HUMIDITY");

  tft.drawLine(0, 27, 128, 27, ST77XX_CYAN);

  tft.setCursor(10, 40);
  tft.setTextColor(ST77XX_RED);
  tft.print("Temp: ");
  tft.setTextColor(ST77XX_WHITE);
  tft.print(g_temp);
  tft.println(" C");

  tft.setCursor(10, 60);
  tft.setTextColor(ST77XX_RED);
  tft.print("Hum : ");
  tft.setTextColor(ST77XX_WHITE);
  tft.print(g_hum);
  tft.println(" %");

  tft.setCursor(10, 145);
  tft.print("<-- Back");
  delay(700);
}

void showAirQuality() {
  tft.fillScreen(ST77XX_BLACK);
  tft.setTextColor(ST77XX_YELLOW);
  tft.setCursor(10, 10);
  tft.println("AIR QUALITY");

  tft.drawLine(0, 27, 128, 27, ST77XX_CYAN);

  tft.setCursor(10, 40);
  tft.setTextColor(ST77XX_RED);
  tft.print("CO2: ");
  tft.setTextColor(ST77XX_WHITE);
  tft.print(g_co2);
  tft.println(" ppm");

  tft.setCursor(10, 60);
  tft.setTextColor(ST77XX_RED);
  tft.print("VOC: ");
  tft.setTextColor(ST77XX_WHITE);
  tft.print(g_voc);
  tft.println(" ppb");

  tft.setCursor(10, 145);
  tft.print("<-- Back");
  delay(700);
}

void showBarometer() {
  tft.fillScreen(ST77XX_BLACK);
  tft.setTextColor(ST77XX_YELLOW);
  tft.setCursor(10, 10);
  tft.println("BAROMETER");

  tft.drawLine(0, 27, 128, 27, ST77XX_CYAN);

  tft.setCursor(10, 40);
  tft.setTextColor(ST77XX_RED);
  tft.print("Pres: ");
  tft.setTextColor(ST77XX_WHITE);
  tft.print(g_pres);
  tft.println(" hPa");

  tft.setCursor(10, 60);
  tft.setTextColor(ST77XX_RED);
  tft.print("Alt : ");
  tft.setTextColor(ST77XX_WHITE);
  tft.print(g_alt);
  tft.println(" m");

  tft.setCursor(10, 145);
  tft.print("<-- Back");
  delay(700);
}

void showWind() {
  tft.fillScreen(ST77XX_BLACK);
  tft.setTextColor(ST77XX_YELLOW);
  tft.setCursor(10, 10);
  tft.println("WIND SPEED");

  tft.drawLine(0, 27, 128, 27, ST77XX_CYAN);

  tft.setCursor(10, 40);
  tft.setTextColor(ST77XX_RED);
  tft.print("Speed: ");
  tft.setTextColor(ST77XX_WHITE);
  tft.print(g_wind);
  tft.println(" km/h");

  tft.setCursor(10, 145);
  tft.print("<-- Back");
  delay(700);
}

void showConnection() {
  tft.fillScreen(ST77XX_BLACK);
  tft.setTextColor(ST77XX_YELLOW);
  tft.setCursor(10, 10);
  tft.println("CONNECTION");

  tft.drawLine(0, 27, 128, 27, ST77XX_CYAN);

  tft.setCursor(10, 45);
  tft.setTextColor(ST77XX_RED);
  tft.print("WiFi: ");
  tft.setTextColor(ST77XX_WHITE);
  if (WiFi.status() == WL_CONNECTED) tft.println("Connected");
  else tft.println("Not connected");

  tft.setCursor(10, 70);
  tft.setTextColor(ST77XX_RED);
  tft.print("MQTT: ");
  tft.setTextColor(ST77XX_WHITE);
  if (mqtt.connected()) tft.println("Connected");
  else tft.println("Not connected");

  tft.setCursor(10, 145);
  tft.print("RIGHT = Connect");

  delay(300);
}

void setup() {
  Serial.begin(9600);

  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH);

  tft.initR(INITR_BLACKTAB);
  tft.setRotation(0);

  pinMode(BTN_UP, INPUT_PULLUP);
  pinMode(BTN_DOWN, INPUT_PULLUP);
  pinMode(BTN_LEFT, INPUT_PULLUP);
  pinMode(BTN_RIGHT, INPUT_PULLUP);

  Wire.begin();
  dht.begin();
  sgp.begin();
  sgp.IAQinit();
  hp.begin();
  analogReadResolution(12);

  drawMenu();
}

void loop() {
  if (millis() - lastRead > 1000) {
    updateAllSensors();
    lastRead = millis();
  }

  if (allowConnection && mqtt.connected()) {
    if (millis() - lastSend > 10000) {
      publishWeather(g_temp, g_hum, g_co2, g_voc, g_pres, g_alt, g_wind);
      sendToBackend();
      lastSend = millis();     
    }                       
  }

  if (allowConnection && WiFi.status() == WL_CONNECTED) {
    mqtt.loop();
  }

  if (screen == 0) {
    if (pressed(BTN_UP)) {
      menuIndex--;
      if (menuIndex < 0) menuIndex = MENU_COUNT - 1;
      drawMenu();
    }

    if (pressed(BTN_DOWN)) {
      menuIndex++;
      if (menuIndex >= MENU_COUNT) menuIndex = 0;
      drawMenu();
    }

    if (pressed(BTN_RIGHT)) {
      screen = menuIndex + 1;
      rightWasPressed = true;
      delay(150);
    }
  }

  else {
    if (pressed(BTN_LEFT)) {
      screen = 0;
      drawMenu();
      delay(150);
    }

    else if (screen == 1) showTempHum();
    else if (screen == 2) showAirQuality();
    else if (screen == 3) showBarometer();
    else if (screen == 4) showWind();

    else if (screen == 5) {
      bool rightNow = pressed(BTN_RIGHT);
      bool newRight = (rightNow && !rightWasPressed);
      rightWasPressed = rightNow;                        //after opening connection when if right button is clicked, will attempt to connect wifi and sent data to backend and mqtt

      if (newRight) {
        allowConnection = true;
      }

      if (allowConnection) {
        if (WiFi.status() != WL_CONNECTED) connectWiFi();
        if (WiFi.status() == WL_CONNECTED && !mqtt.connected()) connectMQTT();
      }

      showConnection();
    }
  }
}
