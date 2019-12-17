/*********************************************************************
This is an example for our Monochrome OLEDs based on SSD1306 drivers

  Pick one up today in the adafruit shop!
  ------> http://www.adafruit.com/category/63_98

This example is for a 64x48 size display using I2C to communicate
3 pins are required to interface (2 I2C and one reset)

Adafruit invests time and resources providing this open source code,
please support Adafruit and open-source hardware by purchasing
products from Adafruit!

Written by Limor Fried/Ladyada  for Adafruit Industries.
BSD license, check license.txt for more information
All text above, and the splash screen must be included in any redistribution
*********************************************************************/

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WEMOS_DHT12.h>

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFiMulti.h>


#include <NTPClient.h>
#include <WiFiUdp.h>

#define STATION_TAG "DHT11"

#define T1_XSIZE    (6)
#define T1_XMAX      (10)
#define T1_XPOS(pos)  (T1_XSIZE*pos) //

#define T1_YSIZE    (8)
#define T1_YMAX     (6)
#define T1_YPOS(pos)  (T1_YSIZE*pos)

#define T2_XSIZE    (8)
#define T2_XMAX     (8)
#define T2_XPOS(pos)  (T2_XSIZE*pos) //

#define T2_YSIZE    (16)
#define T2_YMAX     (3)
#define T2_YPOS(pos)  (T2_YSIZE*pos)


const long utcOffsetInSeconds = 3600*9;
String host = "http://192.168.123.216"; // mysql server

const long Postinterval = 30 * 60 * 1000;  // 30 min.
const long NTPSyncinterval = 30 * 60 * 1000; // 1 min
const long HTinterval = 10 * 1000;  // 10 sec.
const long LEDUpdateinterval = 500; //  0.5 sec

unsigned long prevPostMillis = 0;
unsigned long prevNTPSyncMillis = 0;
unsigned long prevHTMillis = 0;
unsigned long prevLEDUpdateMillis = 0;

float humidity, temp;
float pm10, pm2_5, pm1_0;
String station = STATION_TAG;

unsigned long long ullMSecTicks = 0;
unsigned long long ullEpochTime = 0;
unsigned long long ullTmpEpochTime = 0;

unsigned int tmpsec = 0;
unsigned int tmpmin = 0;


WiFiServer server(80);
WiFiClient client;
HTTPClient http;
ESP8266WiFiMulti wifiMulti;

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "asia.pool.ntp.org", utcOffsetInSeconds);


extern unsigned int gunYear;
extern unsigned int gunMonth;
extern unsigned int gunYDay;
extern unsigned int gunMDay;
extern unsigned int gunHours;
extern unsigned int gunMinutes;
extern unsigned int gunSeconds;
extern unsigned int gunWeekDay;

extern bool bRetyToConnect;

// SCL GPIO5
// SDA GPIO4
#define OLED_RESET 0  // GPIO0
Adafruit_SSD1306 display(OLED_RESET);

DHT12 dht12;

void setup()   {
  
  Serial.begin(115200);

  SSD1306_Setup();

  PrepareToConnect();

//  NTPClient_Setup();

  Serial.println("Setup() done !!");
}

void loop() {

  if( bRetyToConnect == true && !IsWifiConnected() ) {
    Serial.print("Re-connect to Network !!");
    ConnectToNetwork();
  } else {

  }

  // 센서값 DB 전송 부분
  unsigned long currentMillis = millis();
  unsigned long deltaMillis;

  if((deltaMillis = currentMillis - prevLEDUpdateMillis) >= LEDUpdateinterval) {
    
    prevLEDUpdateMillis = currentMillis;

//    ullMSecTicks += LEDUpdateinterval;
    ullMSecTicks += deltaMillis;

//    SerialPrintULL(ullTmpEpochTime);
    ullTmpEpochTime = ullEpochTime + ullMSecTicks / 1000;
//    Serial.print("* ullEpochTime : "); SerialPrintULL(ullEpochTime);    
    secs_to_tm(ullTmpEpochTime);

#if 0
    Serial.print("ullMSecTicks : "); SerialPrintULL(ullMSecTicks);
    Serial.print("gunMinutes: "); Serial.println(gunMinutes);
    Serial.print("gSeconds  : "); Serial.println(gunSeconds);
#endif

    display.setTextSize(1);

    display.fillRect(T1_XPOS(0), T1_YPOS(0), T1_XSIZE*T1_XMAX, T1_YSIZE*2, BLACK);  // x, y, x-width, y-height

    display.setCursor(T1_XPOS(0), T1_YPOS(0));
    display.setTextColor(WHITE);
    display.print(gunYear+1900); display.print("/"); display.print(gunMonth+1); display.print("/"); display.print(gunMDay);

    display.setCursor(T1_XPOS(0), T1_YPOS(1));  // x, y
    display.setTextColor(WHITE);
    if( gunHours < 10 ) display.print("0"); // print leading 0 for beautify the output
    display.print(gunHours); display.print(":");
    if( gunMinutes < 10 ) display.print("0");
    display.print(gunMinutes); display.print(":");
    if( gunSeconds < 10 ) display.print("0");
    display.println(gunSeconds);

    display.display();
  }

  if(currentMillis - prevNTPSyncMillis >= NTPSyncinterval) {  // NTP Sync
    prevNTPSyncMillis = currentMillis;

    timeClient.update();

    ullEpochTime = timeClient.getEpochTime();
    ullMSecTicks = 0; // reset it
    secs_to_tm(ullEpochTime);

#if 1
    Serial.print(STATION_TAG); Serial.println("-------------------------------");
    Serial.print("Time - "); Serial.print(timeClient.getHours()); Serial.print(":"); 
    Serial.print(timeClient.getMinutes()); Serial.print(".");
    Serial.print(timeClient.getSeconds()); Serial.println("");
    Serial.print("Epoch Time - "); Serial.println(timeClient.getEpochTime());

    Serial.print(gunHours); Serial.print(":"); Serial.print(gunMinutes); Serial.print("."); Serial.println(gunSeconds);
#endif
  }

  if(currentMillis - prevHTMillis >= HTinterval) {  // update humidity&temp
    prevHTMillis = currentMillis;

    if(dht12.get()==0){                                                               

      humidity = dht12.humidity;
      temp = dht12.cTemp;  

      display.fillRect(T1_XPOS(0), T1_YPOS(2), T1_XSIZE*T1_XMAX, T1_YSIZE*1, BLACK);
      display.setTextColor(BLACK, WHITE);

      display.setCursor(T1_XPOS(0), T1_YPOS(2));
      display.setTextSize(1);
      display.print(" "); display.print(temp,0); display.print("C  "); display.print(humidity,0); display.print("% ");
    } else {
      display.println("Error!");
    }

    display.display();
  }

  if(currentMillis - prevPostMillis >= Postinterval) {
    prevPostMillis = currentMillis;
    
    String phpHost = host+"/insert_v2.php?temp="+String(temp)+"&humidity="+String(humidity)+"&station="+String(station)+"&pm1_0="+String(0)+"&pm2_5="+String(0)+"&pm10="+String(0);
    Serial.print("[WSM] Connect to ");
    Serial.println(phpHost);
    Serial.print("[WSM] Temperature : "); Serial.println(temp);
    Serial.print("[WSM] Humidity    : "); Serial.println(humidity);
    Serial.print("[WSM] Station     : "); Serial.println(station);
    http.begin(client, phpHost);
    http.setTimeout(1000);
    int httpCode = http.GET();
   
    if(httpCode > 0) {
      Serial.printf("[WSM] GET code : %d\n\n", httpCode);
 
      if(httpCode == HTTP_CODE_OK) {
        String payload = http.getString();
        Serial.println(payload);
      }
    } 
    else {
      Serial.printf("[WSM] GET failed, error: %s\n", http.errorToString(httpCode).c_str());
    }
    http.end();
  }

  // 웹서버 부분
  // if no client (web connection from other client, here this is server(web server host)) available, just return.
  client = server.available();  
  if(!client) return;
 
  Serial.println("[WSM] 새로운 클라이언트");
#if 0
  client.setTimeout(500);
 
  String request = client.readStringUntil('\r');
  Serial.println("[WSM] request: ");
  Serial.println(request);
 
  while(client.available()) {
    client.read();
  }
#else
  client.setTimeout(5000);  
  while (client.connected()) {
    String line = client.readStringUntil('\n');
    if (line == "\r") {
      Serial.println("headers received");
      break;
    } else {
      Serial.println("[WSM] request: ");
      Serial.println(line);
    }
  }
#endif


  if(dht12.get()==0){
    humidity = dht12.humidity;
    temp = dht12.cTemp;  
  } else {

  }

  client.print("HTTP/1.1 200 OK");
  client.print("Content-Type: text/html\r\n\r\n");
  client.print("<!DOCTYPE HTML>");
  client.print("<html>");
  client.print("<head>"); 
  client.print("<meta charset=\"UTF-8\" http-equiv=\"refresh\" content=\"1\">");
  client.print("<title>DHT11 senrsor test Webpage</title>");
  client.print("</head>");
  client.print("<body>");
  client.print("<h2>DHT11 senrsor test Webpage</h2>");
  client.print("<br>");
  client.print("Temperature : ");
  client.print(temp);
  client.print(" °C");
  client.print("<br>");
  client.print("Humidity : ");
  client.print(humidity);
  client.print(" %");
  client.print("<br>");  
  client.print("Last update on LED : ");
  client.print(gunYear+1900); client.print("/");
  client.print(gunMonth+1); client.print("/");
  client.print(gunMDay); client.print(" ");
  client.print(gunHours); client.print(":");
  client.print(gunMinutes); client.print(":");
  client.print(gunSeconds);
  client.print("</body>");
  client.print("</html>");

  Serial.println("[WSM] 클라이언트 연결 해제");

//  delay(1000);
}
