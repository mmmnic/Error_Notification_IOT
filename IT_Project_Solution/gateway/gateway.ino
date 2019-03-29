#include <ESP8266WiFi.h>
#include <Arduino_JSON.h>

const char* ssid = "UIT_Guest";
const char* password = "1denmuoi1";
const char* host = "192.168.0.100";
WiFiClient client;
const int httpPort = 8080;

int temperature, humidity;
String jsonString = "";
JSONVar myITSolution;

void setup() {
  Serial.begin(9600);
  Serial.println();

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

  
//  Serial.println("Connected to Server");
}

void loop() {

//  Serial.println("Connected to server");
  if(Serial.available()>0)    //Checks is there any data in buffer 
  {
    //Serial.print("We got:");
    char character = char(Serial.read());
    jsonString += character;
    //Serial.print(character);  //Read serial data byte and send back to serial monitor
    if (character == '}')
    {
      Serial.println(jsonString);
      myITSolution = readJson(jsonString);
      Serial.println((double) myITSolution["HUMIDITY"]);
      Serial.println((double) myITSolution["TEMPERATURE"]);

      postToServer();
      jsonString = "";
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
  String uri = "http://192.168.0.100:8080/showData.php";
  String temp = String((double)myITSolution["TEMPERATURE"]);
  String hum = String((double)myITSolution["HUMIDITY"]);
  String nodeID = String((const char*) myITSolution["NODEID"]);
  // chuỗi data sau khi đã lấy ra từ json
  String data = "temperature=" + temp + "&humidity=" + hum + "&nodeid=" + nodeID;
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

//  while (client.connected() || client.available())
//    {
//      if (client.available())
//      {
//        String line = client.readStringUntil('\n');
//        Serial.println(line);
//      }
//    }
}

// {"HUMIDITY":49.99998472,"TEMPERATURE":26.5}
JSONVar readJson(String jsonString)
{
  JSONVar myObject = JSON.parse(jsonString);
  return myObject;
}
