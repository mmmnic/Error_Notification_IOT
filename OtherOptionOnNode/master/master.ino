#include "ESP8266WiFi.h"
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <String.h>
 
const char* ssid = "UIT_Guest";
const char* password = "1denmuoi1";
const char* host = "192.168.0.113";
 
WiFiClient client;
const int httpPort = 8080;

char MACHINE_ID[10];
char MACHINE_INDEX[10];
char ERROR_INDEX[10];

ESP8266WebServer server(80);
//Static IP address configuration
IPAddress staticIP(192, 168, 0, 130); //ESP static ip
IPAddress gateway(192, 168, 0, 1);   //IP Address of your WiFi Router (Gateway)
IPAddress subnet(255, 255, 255, 0);  //Subnet mask
IPAddress dns(8, 8, 8, 8);  //DNS

void setup() {
  Serial.begin(9600);
  Serial.println();
  
  WiFi.disconnect();
  WiFi.config(staticIP, subnet, gateway, dns);

  WiFi.begin(ssid, password);

  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.begin();                  //Start server
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
}

void postToServer()
{
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    delay(1000);
    return;
  }
  // địa chỉ trang cần post
  String uri = "http://192.168.0.100:8080/ErrorNoti.php";
  // chuỗi data sau khi đã lấy ra từ json
  String data = "MACHINE_ID=" + String(MACHINE_ID) + "&MACHINE_INDEX=" + String(MACHINE_INDEX) + "&ERROR_INDEX=" + String(ERROR_INDEX);
  //String data = "pst=temperature>" + String((double)myITSolution["TEMPERATURE"]) + "||humidity>" + String((double)myITSolution["HUMIDITY"]) + "||data>text" + "||nodeid>NODE1";
  Serial.print("Requesting POST: ");
  // gửi request post tới server (dưới đây là 1 chuỗi quy tắc post):
  client.println("POST " + uri + " HTTP/1.0");
  client.println("Host: " + String(host));
  client.println("Accept: */*");
  client.print("Content-Length: ");
  client.println(data.length());
  client.println("Content-Type: application/x-www-form-urlencoded");
  client.println();
  client.print(data);
}

void handleRoot() {

  //strcpy(connect_ssid, server.arg("ip"));
  server.arg("MACHINE_ID").toCharArray(MACHINE_ID, server.arg("MACHINE_ID").length()+1);
  server.arg("MACHINE_INDEX").toCharArray(MACHINE_INDEX, server.arg("MACHINE_INDEX").length()+1);
  server.arg("ERROR_INDEX").toCharArray(ERROR_INDEX, server.arg("ERROR_INDEX").length()+1);
  Serial.println(MACHINE_ID);
  //Serial.println(server.arg("ip").length());
  Serial.println(MACHINE_INDEX);
  Serial.println(ERROR_INDEX);

  postToServer();
  
  server.send(200, "text/html", 
  "<!DOCTYPE html>\
    <html lang=\"en\">\
    <head>\
      <meta charset=\"UTF-8\">\
      <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\
      <style>\
        body {font-family: Arial, Helvetica, sans-serif;}\
        * {box-sizing: border-box;}\
   \
        input[type=text], select, textarea {\
          width: 100%;\
          padding: 12px;\
          border: 1px solid #ccc;\
          border-radius: 4px;\
          box-sizing: border-box;\
          margin-top: 6px;\
          margin-bottom: 16px;\
          resize: vertical;\
        }\
        \
        input[type=password], select, textarea {\
          width: 100%;\
          padding: 12px;\
          border: 1px solid #ccc;\
          border-radius: 4px;\
          box-sizing: border-box;\
          margin-top: 6px;\
          margin-bottom: 16px;\
          resize: vertical;\
        }\
    \
        input[type=submit] {\
          background-color: #4CAF50;\
          color: white;\
          padding: 12px 20px;\
          border: none;\
          border-radius: 4px;\
          cursor: pointer;\
        }\
        \
        button[type=submit] {\
          background-color: #4CAF50;\
          color: white;\
          padding: 12px 20px;\
          border: none;\
          border-radius: 4px;\
          cursor: pointer;\
        }\
    \
        input[type=submit]:hover {\
          background-color: #45a049;\
        }\
    \
        .container {\
          border-radius: 5px;\
          background-color: #f2f2f2;\
          padding: 20px;\
        }\
      </style>\
      <title>My Smart Home</title>\
    </head>\
    <body>\
      <div class=\"container\">\
        <h2>Smart Config</h2>\
        <form action=\"\" method=\"get\">\
        <div class=\"row\">\
          <div class=\"form-group col-md-12\">\
            <label for=\"ip\">MACHINE_ID:</label>\
            <input type=\"text\" class=\"form-control\" id=\"MACHINE_ID\" name=\"MACHINE_ID\">\
          </div>\
          <div class=\"form-group col-md-12\">\
            <label for=\"pwd\">MACHINE_INDEX:</label>\
            <input type=\"password\" class=\"form-control\" id=\"MACHINE_INDEX\" name=\"MACHINE_INDEX\">\
          </div>\
          <div class=\"form-group col-md-12\">\
            <label for=\"server_ip\">ERROR_INDEX:</label>\
            <input type=\"text\" class=\"form-control\" id=\"ERROR_INDEX\" name=\"ERROR_INDEX\">\
          </div>\
          <div class=\"form-group col-md-12\">\
            <button type=\"submit\" class=\"btn btn-success\">Submit</button>\
          </div>\
        </div>\
        </form>\
      </div>\
    </body>\
    </html>"
  );
}
