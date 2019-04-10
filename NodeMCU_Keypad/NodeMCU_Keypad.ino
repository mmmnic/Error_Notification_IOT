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

const char *host = "http://192.168.1.4:8080/Error_Notification_IOT/ErrorNoti.php";

const byte rows = 4; //số hàng
const byte columns = 4; //số cột
 
int holdDelay = 700; //Thời gian trễ để xem là nhấn 1 nút nhằm tránh nhiễu
int n = 3; // 
int state = 0; //nếu state =0 ko nhấn,state =1 nhấn thời gian nhỏ , state = 2 nhấn giữ lâu
char key = 0;
 
//Định nghĩa các giá trị trả về
char keys[rows][columns] =
{
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'},
};
 
byte rowPins[rows] = {D0, D1, D2 , D3}; //Cách nối chân với Arduino
byte columnPins[columns] = {D4, D5, D6, D7};
 
//cài đặt thư viện keypad
Keypad keypad = Keypad(makeKeymap(keys), rowPins, columnPins, rows, columns);
HTTPClient http; 

void setup() {
  Serial.begin(9600);//bật serial, baudrate 9600

  // Ket noi wifi
  connectWifi();

  // ket noi den web
  http.begin(host);  

}
void loop() {  
  String postData = "MACHINE_ID:" + MACHINE_ID + "MACHINE_INDEX:" + MACHINE_INDEX + "ERROR_INDEX:";
  char temp = keypad.getKey();

  
  if ((int)keypad.getState() ==  PRESSED) {
    if (temp != 0) {
      key = temp;
    }
  }
 
  if ((int)keypad.getState() ==  RELEASED) {
    key += state;
    state = 0;
    
    postData += key;
    http.POST(postData);
  }
  delay(100);
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
