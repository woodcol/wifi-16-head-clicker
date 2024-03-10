
#ifndef configData_h
#define configData_h 

#define LED_BUILTIN 2 

#define printSerial 0

typedef struct _wifi_network_info {
    char ssid[33];
    int rssi;
    int secure;

    struct _wifi_network_info *next;
} wifi_network_info_t;

#define HeaveBase64 1 //数据是否base64格式
#define heaveCpy    1 //数据是否加密,只要是加密,则数据一定是base64

#define DataBuffize 64

#if heaveCpy
    #define HeaveBase64 1
#endif 

#endif
