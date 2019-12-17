#include "config.h"

/* Delay paramter for connection. */
#define WIFI_DELAY        500
/* Wait this much until device gets IP. */
#define MAX_CONNECT_TIME  10000   // 10 sec.

const char* ssid     = "YourSSID";
const char* password = "YourPASSWORD";

bool bRetyToConnect = true;
bool bFirstTry = true;

int IsWifiConnected(void)
{
  // Patch
  if( bFirstTry ) {
    bFirstTry = false;
    return 0;
  }
#if 0
  if( wifiMulti.run() == WL_CONNECTED )
    return 1;
  else
    return 0;
#else
  if( WiFi.status() == WL_CONNECTED )
    return 1;
  else
    return 0;
#endif
}

void PrepareToConnect()
{
  WiFi.mode(WIFI_STA);
}

void ConnectToNetwork() 
{
  unsigned int connecting_progress = 0;
  unsigned int unRetryCount = 0;

  server.close();
  WiFi.disconnect();

#if 1 // Debug on serial
  Serial.print("Connect to ");
  Serial.println(ssid);

#ifdef SSD1306_LCD64X48
  display.fillRect(T1_XPOS(0), T1_YPOS(3), T1_XSIZE*T1_XMAX, T1_YSIZE*3, BLACK);

  display.setCursor(T1_XPOS(0), T1_YPOS(3));
  display.setTextColor(WHITE);
  display.print("Connect to "); display.println(ssid);
  display.display();
#endif  
#endif

  WiFi.begin(ssid, password);

#ifdef SSD1306_LCD64X48
  display.setCursor(T1_XPOS(0), T1_YPOS(5));
#endif

  while (WiFi.status() != WL_CONNECTED && unRetryCount < MAX_CONNECT_TIME) {

    Serial.print("unRetryCount : "); Serial.println(unRetryCount);

    unRetryCount += WIFI_DELAY;
    delay(WIFI_DELAY);
    Serial.print(".");

    Serial.print("WiFi.status()   : "); Serial.println(WiFi.status());
//    Serial.print("wifiMulti.run() : "); Serial.println(wifiMulti.run());

#ifdef SSD1306_LCD64X48
    if( connecting_progress++ % 2 ) 
      display.print(".");
    else
      display.print("*");
    display.display();

    if( connecting_progress > 9 ) {
      connecting_progress = 0; // not to print out in a new line
      display.setCursor(T1_XPOS(0), T1_YPOS(5));
      display.fillRect(T1_XPOS(0), T1_YPOS(5), T1_XSIZE*T1_XMAX, T1_YSIZE, BLACK);
    }
#endif    
  }

  if( unRetryCount >= MAX_CONNECT_TIME ) {
    return;
  }

//  bRetyToConnect = false;

#if 1
  Serial.print("IP address: "); Serial.println(WiFi.localIP());
  Serial.print("Channel   : "); Serial.println(WiFi.channel());
  Serial.print("SSID      : "); Serial.println(WiFi.SSID());

#ifdef SSD1306_LCD64X48
  display.fillRect(T1_XPOS(0), T1_YPOS(3), T1_XSIZE*T1_XMAX, T1_YSIZE*3, BLACK);
  display.setCursor(T1_XPOS(0), T1_YPOS(3));
  display.setTextSize(1);
  display.setTextColor(WHITE);  
  display.print(WiFi.localIP()); display.print("/"); display.print(WiFi.SSID()); display.print("("); display.print(WiFi.channel()); display.println(")");
#endif
#endif 

  server.begin();

  NTPClient_Setup();
}
