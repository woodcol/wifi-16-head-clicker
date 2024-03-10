#include "webConfigData.h"
#ifdef ESP32
#include <WiFi.h>
#include <WebServer.h>
#else
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#endif
#include "indexhtml.h"
#include "WifiScanUtil.h"

const char *html_settings_header = 
"<!DOCTYPE html>"
"<html>"
"<head>"
"<title>WiFi Configration</title>"
"<style type=\"text/css\">"
"html, input {font-size: 40px;}"
"h1 { font-size: 150%; text-align: center; }"
"body .container {width: 15em; margin: 0 auto; padding: 1em;}"
"label {display: block;}"
"input[type=text], input[type=password] {width: 100%; box-sizing: border-box;}"
"input[type=submit] {"
"    width: 100%;"
"    border: 1px solid black;"
"    text-decoration: none;"
"    padding: 0.25em 5em;"
"    color: white;"
"    background-color: #4caf50;"
"    cursor: pointer;"
"    -webkit-appearance: none;"
"    -webkit-border-radius: 0;"
"}"
"input[type=submit]#refresh {background-color: #44bbe3;}"
"input[type=submit]#join {background-color: #4caf50;}"
"input[type=submit][disabled] {"
"    color: #aaaaaa;"
"    background-color: #eeeeee !important;"
"}"
".networks {list-style: none; margin: 1em 0; padding: 0;}"
".networks li {"
"    cursor: pointer;"
"    padding: 0.25em;"
"    margin-bottom: 0.25em;"
"    white-space: nowrap;"
"    overflow: hidden;"
"    text-overflow: ellipsis;"
"    position: relative;"
"    padding-left: 1.1em;"
"}"
".networks li.secure::before {content: \"🔒\"; position: absolute; left: 0;}"
".networks li.selected {font-weight: bold; background-color: #ddd}"
".networks li:hover {background-color: #eee;}"
".networks li.other {border-top: 2px solid #aaa; text-align: center;}"
".nonetworks {text-align: center; margin: 1em 0;}"
".field {margin-bottom: 0.5em;}"
".field.required label::before {content: \"*\"; color: red;}"
"</style>"
"</head>"
"<body>"
"    <div class=\"container\">"
"        <h1>Join WiFi network</h1>"
"        <form action=\"/settings\" method=\"get\">"
"            <input type=\"submit\" id=\"refresh\" value=\"⟲ Refresh\" />"
"        </form>"
"        <form action=\"/settings\" method=\"post\">"
"            <div class=\"nonetworks\" style=\"display: none;\">"
"                No networks found"
"            </div>"
"            <ul id=\"networks\" class=\"networks\">";



const char *html_network_item = 
"                <li class=\"%s\">%s</li>";

const char *html_settings_footer = 
"                <li class=\"other\">Other</li>"
"            </ul>"
"            <div class=\"field required ssid\" style=\"display: none;\">"
"                <label for=\"ssid\">SSID:</label>"
"                <input type=\"text\" id=\"ssid\" name=\"ssid\" />"
"            </div>"
"            <div class=\"field required password\" style=\"display: none;\">"
"                <label for=\"password\">Password:</label>"
"                <input type=\"password\" id=\"password\" name=\"password\" />"
"            </div>"
"            <input type=\"submit\" id=\"join\" value=\"Join\" disabled />"
"        </form>"
"    </div>"
"    <script>"
"var ssid_field, password_block, password_field, join_button;"
"networks_block = document.getElementById('networks');"
"ssid_block = document.querySelector('.field.ssid');"
"ssid_field = document.getElementById('ssid');"
"password_block = document.querySelector('.field.password');"
"password_field = document.getElementById('password');"
"join_button = document.getElementById('join');"
"function enable(e) { e.disabled = ''; }"
"function disable(e) { e.disabled = 'disabled'; }"
"function show(e) { e.style.display = 'block'; }"
"function hide(e) { e.style.display = 'none'; }"
"function selectNetwork(e) {"
"    let nets = document.querySelectorAll('ul.networks li');"
"    for (var i=0; i<nets.length; i++) {"
"        nets[i].classList.remove('selected');"
"    }"
"    e.classList.add('selected');"
"}"
"var nets, i;"
"nets = document.querySelectorAll('ul.networks li.unsecure');"
"for (i=0; i<nets.length; i++) {"
"    nets[i].onclick = function(event) {"
"        event.preventDefault();"
"        selectNetwork(this);"
"        ssid_field.value = this.innerText;"
"        hide(ssid_block);"
"        hide(password_block);"
"        disable(password_field);"
"        password_field.value = '';"
"        enable(join_button);"
"    }"
"}"
"nets = document.querySelectorAll('ul.networks li.secure');"
"for (i=0; i<nets.length; i++) {"
"    nets[i].onclick = function(event) {"
"        event.preventDefault();"
"        selectNetwork(this);"
"        ssid_field.value = this.innerText;"
"        hide(ssid_block);"
"        enable(password_field);"
"        show(password_block);"
"        disable(join_button);"
"        password_block.classList.add('required');"
"    }"
"}"
"nets = document.querySelectorAll('ul.networks li.other');"
"for (i=0; i<nets.length; i++) {"
"    nets[i].onclick = function(event) {"
"        event.preventDefault();"
"        if (this.classList.contains('selected'))"
"            return;"
"        selectNetwork(this);"
"        ssid_field.value = \"\";"
"        show(ssid_block);"
"        show(password_block);"
"        enable(ssid_field);"
"        enable(password_field);"
"        disable(join_button);"
"        password_block.classList.remove('required');"
"    }"
"}"
"if (document.querySelectorAll('ul.networks li').length == 1) {"
"    show(document.querySelector('.nonetworks'));"
"    hide(networks_block);"
"    show(ssid_block);"
"    show(password_block);"
"    password_block.classList.remove('required');"
"}"
"let s = ssid_field;"
"let p = password_field;"
"s.onchange = s.onkeydown = s.onpaste = s.oninput ="
"p.onchange = p.onkeydown = p.onpaste = p.oninput = function(event) {"
"    var f = enable;"
"    if (ssid_block.classList.contains('required') && ssid_field.value == '') {"
"        f = disable;"
"    }"
"    if (password_block.classList.contains('required') && password_field.value == '') {"
"        f = disable;"
"    }"
"    f(join_button);"
"}"
"    </script>"
"</body>"
"</html>";


extern wifi_network_info_t *wifi_networks;

#ifdef ESP32
void sendWifiConfigHtml(WebServer *cli)
#else
void sendWifiConfigHtml(ESP8266WebServer *cli)
#endif

{
  static const char http_prologue[] =
        "HTTP/1.1 200 \r\n"
        "Content-Type: text/html; charset=utf-8\r\n"
        "Cache-Control: no-store\r\n"
        "\r\n";
  cli->sendContent(http_prologue);
  cli->sendContent(html_settings_header);
  char buff[64];
  
  //变量上线程锁
  wifi_network_info_t *net = wifi_networks;
  while (net) {
      snprintf(
          buff, sizeof(buff),
          html_network_item,
          net->secure ? "secure" : "unsecure", net->ssid
      );
      // cli.print(buff);
      cli->sendContent(buff);
      net = net->next;
  }
  // cli.print(html_settings_footer);
  cli->sendContent(html_settings_footer);
  cli->sendContent("\r\n");

}

const char *html_works_header = 
"<!DOCTYPE html>"
"<html>"
"<head>"
"<title>wifi touch</title>"
"</head>"
"<body>"
"硬件购买地址:"
"<a href=\"https://fengmm521.taobao.com/\">https://fengmm521.taobao.com/</a>"
"</body>"
"</html>";

#ifdef ESP32
void sendWifiWorkHtml(WebServer *cli)
#else
void sendWifiWorkHtml(ESP8266WebServer *cli)
#endif
{
  static const char http_prologue[] =
        "HTTP/1.1 200 \r\n"
        "Content-Type: text/html; charset=utf-8\r\n"
        "Cache-Control: no-store\r\n"
        "\r\n";
  cli->sendContent(http_prologue);
  cli->sendContent(html_works_header);
  cli->sendContent("\r\n");

}
