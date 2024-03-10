#include <ESP8266WiFi.h>
#include <WiFiClient.h>
// #include <WebServer.h>
#include <ESP8266WebServer.h>
// #include <ESPmDNS.h>
#include <ESP8266mDNS.h>
#include "webConfigData.h"

#include "WifiScanUtil.h"

wifi_network_info_t *wifi_networks;

bool scanruning = false;



void serial_showScan()
{
    
  wifi_network_info_t *net = wifi_networks;
  int i = 1;
  while(net){
      // statement
      Serial.print(i);
      i++;
      Serial.print(": ");
      Serial.print(net->ssid);
      Serial.print(" (");
      Serial.print(net->rssi);
      Serial.print(")");
      Serial.println((net->secure)?"*":" ");
      delay(10);
      net = net->next;
  }
  
}

void wifi_scanrun()
{
    // WiFi.scanNetworks will return the number of networks found
    Serial.println("start scan");
    int n = WiFi.scanNetworks();
    if (n == 0) {
        Serial.println("no networks found");
    } else {
        Serial.print(n);
        Serial.println(" networks found");

        wifi_network_info_t *wifi_network = wifi_networks;
        while (wifi_network) {
            wifi_network_info_t *next = wifi_network->next;
            free(wifi_network);
            wifi_network = next;
        }
        wifi_networks = NULL;

        for (int i = 0; i < n; ++i) {
          wifi_network_info_t *net = wifi_networks;
          while (net) {//删除重复的ssid
              if (!strncmp(net->ssid, (char *)WiFi.SSID(i).c_str(), sizeof(net->ssid)))
                  break;
              net = net->next;
          }
          if(!net){
            wifi_network_info_t* net = new wifi_network_info_t();
            memset(net, 0, sizeof(*net));
            strncpy(net->ssid, WiFi.SSID(i).c_str(), sizeof(net->ssid));
            net->secure = WiFi.encryptionType(i) != ENC_TYPE_NONE;
            net->rssi = WiFi.RSSI(i);
            net->next = wifi_networks;
            wifi_networks = net;
          }
            
        }

    }
}
