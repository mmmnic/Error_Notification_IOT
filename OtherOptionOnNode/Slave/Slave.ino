#include "ESP8266WiFi.h"

const char* ssid = "UIT_Guest";
const char* password = "1denmuoi1";
const char* host = "192.168.0.130";

char MACHINE_ID[10] = "1";
char MACHINE_INDEX[10]= "1";
char ERROR_INDEX[10] = "1";

const int httpPort = 80;

WiFiClient client;

void setup() {
  Serial.begin(9600);
  Serial.println();
  
  WiFi.disconnect();

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
}

void loop() {
   if(Serial.available()>0)    //Checks is there any data in buffer 
  {
//    Serial.print("We got:");
    char character = char(Serial.read());
//    jsonString += character;
    Serial.print(character);  //Read serial data byte and send back to serial monitor
    if (character == '}')
    {
//      Serial.println(jsonString);
//      myITSolution = readJson(jsonString);
//      Serial.println((double) myITSolution["HUMIDITY"]);
//      Serial.println((double) myITSolution["TEMPERATURE"]);

      postToServer();
//      jsonString = "";
    }
  }
}

void postToServer()
{
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    delay(1000);
    return;
  }
  // địa chỉ trang cần post
  String uri = "http://192.168.0.130";
  // chuỗi data sau khi đã lấy ra từ json
  String data = "GET /?" "MACHINE_ID=" + String(MACHINE_ID) + "&MACHINE_INDEX=" + String(MACHINE_INDEX) + "&ERROR_INDEX=" + String(ERROR_INDEX) + " HTTP/1.0";
  //String data = "pst=temperature>" + String((double)myITSolution["TEMPERATURE"]) + "||humidity>" + String((double)myITSolution["HUMIDITY"]) + "||data>text" + "||nodeid>NODE1";
  Serial.print("Requesting GET: ");
  // gửi request post tới server (dưới đây là 1 chuỗi quy tắc post):
  Serial.println(data);
  client.println(data);
  client.println();
}
