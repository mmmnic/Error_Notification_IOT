#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <Keypad.h>

String MACHINE_ID    = "1";
String MACHINE_INDEX = "1";

// Nhap ten wifi va mat khau wifi o day
const char *ssid = "TP-LINK_F608";
const char *password = "quenpassroi";
const char *host = "192.168.1.4";
const int httpPort = 8080;

WiFiClient client;

const byte n_rows = 4;
const byte n_cols = 4;
 
char keys[n_rows][n_cols] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
 
byte colPins[n_rows] = {D3, D2, D1, D0}; //Cách nối chân với nodemcu
byte rowPins[n_cols] = {D7, D6, D5, D4};

//cài đặt thư viện keypad
Keypad myKeypad = Keypad( makeKeymap(keys), rowPins, colPins, n_rows, n_cols); 


void setup() {
  Serial.begin(115200);//bật serial, baudrate 9600

  Serial.println("hello");
  // Ket noi wifi
  connectWifi(); 
}

void loop() {  
  char myKey = myKeypad.getKey();
 
  if (myKey != NULL){
    postServer(String (myKey));
    Serial.println(myKey);
  }
}

// Ham ket noi vao wifi
void connectWifi()
{
  // hien thi mang wifi dang ket noi den
  Serial.print("dang ket noi den wifi: ");
  Serial.println(ssid);

  // bat dau ket noi den wifi
  WiFi.begin(ssid, password);

  // doi ket noi wifi
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println("Dang ket noi");
  }

  // thong bao neu ket noi thanh cong
  Serial.println("Da ket noi wifi thanh cong");
}

void postServer(String ERROR_DATA)
{
  // Ket noi den server
  if (!client.connect(host, httpPort)) {
    Serial.println("Mat ket noi den server");
    return;
  }
  
  // địa chỉ trang cần post
  String uri = "http://192.168.0.119:8080/Error_Notification_IOT/ErrorNoti.php";
  
  // chuỗi data sau khi đã lấy ra từ json
  String data = "MACHINE_ID=" + MACHINE_ID + "&MACHINE_INDEX=" + MACHINE_INDEX + "&ERROR_INDEX=" + ERROR_DATA;
  // gửi request post tới server (dưới đây là 1 chuỗi quy tắc post):
  client.println("POST " + uri + " HTTP/1.0");
  client.println("Host: " + String(host));
  client.println("Accept: /");
  client.print("Content-Length: ");
  client.println(data.length());
  client.println("Content-Type: application/x-www-form-urlencoded");
  client.println();
  client.print(data);
  Serial.println("Gui thanh cong");
}
