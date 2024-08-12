#include "main.h"
#include "flat.h"
#include "pins.h"
bool swflag = false;

WiFiServer tcpServer(TCP_PORT);
WiFiClient tcpClient;

AlpacaServer alpacaServer("AstroFlat");
Flat flat; // create flat object
void setup()
{
  pinMode(swpin, INPUT_PULLUP);
  // setup serial
  Serial.begin(115200, SERIAL_8N1);

  setup_wifi();

  // setup ASCOM Alpaca server
  alpacaServer.begin(ALPACA_UDP_PORT, ALPACA_TCP_PORT);

  flat.begin();
  alpacaServer.addDevice(&flat);

  // load settings
  alpacaServer.loadSettings();
}

void loop()
{
  delay(300);
  // if switch goes from off to on turn on flat full brightness
  if (((digitalRead(swpin)) == 0) && (swflag == false))
  {
    flat.setdimlevel(MaxBrightness);
    flat.pwr_on();
    swflag = true;
  }
  // if switch goes from on to off turn off flat and set to 0 brightness
  if (((digitalRead(swpin)) == 1) && (swflag == true))
  {
    flat.setdimlevel(0);
    flat.pwr_off();
    swflag = false;
  }
}

void setup_wifi()
{
  pinMode(PIN_WIFI_LED, OUTPUT);

  // setup wifi
  Serial.println(F("# Starting WiFi"));

  DoubleResetDetector drd = DoubleResetDetector(DRD_TIMEOUT, DRD_ADDRESS);
  ESP_WiFiManager ESP_wifiManager(HOSTNAME);
  ESP_wifiManager.setConnectTimeout(60);
  Serial.print(F("WiFi SSID "));
  Serial.println(ESP_wifiManager.WiFi_SSID());
  Serial.print(F("SW pin "));
  Serial.println(digitalRead(swpin));
  // if (ESP_wifiManager.WiFi_SSID() == "" || drd.detectDoubleReset())
  //  double reset detector seems to be not working properly
  // power up with switch on to program SSID and PW
  if ((ESP_wifiManager.WiFi_SSID() == "") || (digitalRead(swpin) == 0))
  {
    Serial.println(F("# Starting Config Portal"));
    digitalWrite(PIN_WIFI_LED, HIGH);
    if (!ESP_wifiManager.startConfigPortal())
    {
      Serial.println(F("# Not connected to WiFi"));
    }
    else
    {
      Serial.println(F("# connected"));
    }
  }
  else
  {
    WiFi.mode(WIFI_STA);
    WiFi.begin();
    //}
    WiFi.waitForConnectResult();
    if (WiFi.status() != WL_CONNECTED)
    {
      Serial.println(F("# Failed to connect"));
    }
    else
    {
      Serial.print(F("# Local IP: "));
      Serial.println(WiFi.localIP());
      digitalWrite(PIN_WIFI_LED, HIGH);
      if (!MDNS.begin(HOSTNAME))
      {
        Serial.println("# Error starting mDNS");
        return;
      }
    }
  }
}
