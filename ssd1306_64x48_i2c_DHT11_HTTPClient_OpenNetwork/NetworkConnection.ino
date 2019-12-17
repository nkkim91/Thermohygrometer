#include <ESP8266WiFi.h>

/* Delay paramter for connection. */
#define WIFI_DELAY        500
/* Wait this much until device gets IP. */
#define MAX_CONNECT_TIME  10000   // 10 sec.


bool bRetyToConnect = true;
bool bFirstTry = true;

/* Max SSID octets. */
#define MAX_SSID_LEN      32

/* SSID that to be stored to connect. */
char ssid[MAX_SSID_LEN] = "";

/* Scan available networks and sort them in order to their signal strength. */
void FindOpenNetwork() {

  memset(ssid, 0, MAX_SSID_LEN);
  int n = WiFi.scanNetworks();
  Serial.println("Scan complete!");
  if (n == 0) {
    Serial.println("No networks available.");
  } else {
    Serial.print(n);
    Serial.println(" networks discovered.");
    int indices[n];
    for (int i = 0; i < n; i++) {
      indices[i] = i;
    }
    for (int i = 0; i < n; i++) {
      for (int j = i + 1; j < n; j++) {
        if (WiFi.RSSI(indices[j]) > WiFi.RSSI(indices[i])) {
          std::swap(indices[i], indices[j]);
        }
      }
    }
    Serial.println("----------------------------------------------");
    Serial.println("The available open network is : ");
    for (int i = 0; i < n; ++i) {
      Serial.print(WiFi.SSID(indices[i]));
      Serial.print(" ");
      Serial.print(WiFi.RSSI(indices[i]));
      Serial.print(" ");
      Serial.print(WiFi.encryptionType(indices[i]));
      Serial.println();
      if(WiFi.encryptionType(indices[i]) == ENC_TYPE_NONE) {  // ENC_TYPE_NONE : 7
        memset(ssid, 0, MAX_SSID_LEN);
        strncpy(ssid, WiFi.SSID(indices[i]).c_str(), MAX_SSID_LEN);
        break;
      }
    }
  }
}

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
  WiFi.softAPdisconnect();
  WiFi.disconnect();

  delay(WIFI_DELAY);

ReScan:

  FindOpenNetwork();
  delay(WIFI_DELAY);
  
  if(strlen(ssid) <= 0) {

    Serial.println("No open networks available. :-(");  

    display.fillRect(T1_XPOS(0), T1_YPOS(3), T1_XSIZE*T1_XMAX, T1_YSIZE*3, BLACK);

    display.setCursor(T1_XPOS(0), T1_YPOS(3));
    display.setTextColor(WHITE);
    
    display.println("No open networks available !! Retry now ");
    display.display();
    
    goto ReScan;
  }

#if 1 // Debug on serial
  Serial.print("Connect to ");

  display.fillRect(T1_XPOS(0), T1_YPOS(3), T1_XSIZE*T1_XMAX, T1_YSIZE*3, BLACK);

  display.setCursor(T1_XPOS(0), T1_YPOS(3));
  display.setTextColor(WHITE);

  display.print("Connect to "); display.print(ssid);
  display.display();
#endif


  WiFi.begin(ssid);

  display.setCursor(T1_XPOS(0), T1_YPOS(5));

  while (WiFi.status() != WL_CONNECTED && unRetryCount < MAX_CONNECT_TIME) {

    Serial.print("unRetryCount : "); Serial.println(unRetryCount);
    unRetryCount += WIFI_DELAY;
    delay(WIFI_DELAY);
    Serial.print(".");

    Serial.print("WiFi.status()   : "); Serial.println(WiFi.status());
//    Serial.print("wifiMulti.run() : "); Serial.println(wifiMulti.run());

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
  }

  if( unRetryCount >= MAX_CONNECT_TIME ) {
    return;
  }

//  bRetyToConnect = false;

#if 1
  Serial.print("IP address: "); Serial.println(WiFi.localIP());
  Serial.print("Channel   : "); Serial.println(WiFi.channel());
  Serial.print("SSID      : "); Serial.println(WiFi.SSID());

  display.fillRect(T1_XPOS(0), T1_YPOS(3), T1_XSIZE*T1_XMAX, T1_YSIZE*3, BLACK);
  display.setCursor(T1_XPOS(0), T1_YPOS(3));
  display.setTextSize(1);
  display.setTextColor(WHITE);  
  display.print(WiFi.localIP()); display.print("/"); display.print(WiFi.SSID()); display.print("("); display.print(WiFi.channel()); display.println(")");
#endif 

  server.begin();

  NTPClient_Setup();
}
