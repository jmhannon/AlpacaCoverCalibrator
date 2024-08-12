#pragma once
#include <Arduino.h>
#include "config.h"

#include <ESP_WiFiManager.h>
#include <ESP_DoubleResetDetector.h>
#include <ESPmDNS.h>
#include <SPI.h>
#include <SPIFFS.h>
#include "AlpacaServer.h"

// module setup
void setup_wifi();
