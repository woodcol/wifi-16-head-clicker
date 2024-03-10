#ifndef WIFISCANUTIL_H
#define WIFISCANUTIL_H 


typedef struct _wifi_network_info {
    char ssid[33];
    int rssi;
    int secure;
    struct _wifi_network_info *next;
} wifi_network_info_t;


void wifi_scanrun();


#endif