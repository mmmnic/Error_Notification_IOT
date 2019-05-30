#include <Adafruit_NeoPixel.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <Keypad.h>


// Khai báo số của máy
String MACHINE_ID    = "3";
String MACHINE_INDEX = "3";

// Nhap ten wifi va mat khau wifi o day
const char *ssid = "HUAWEI";
const char *password = "12345678";

// Nhap dia chi IP cua server va port
const char *host = "192.168.43.175";
const int httpPort = 80;

// Khởi tạo client
WiFiClient client;

// khai bao số lượng hàng và cột của keypad
const byte n_rows = 4; 
const byte n_cols = 4;

// định nghĩa vị trí của các nút bấm
char keys[n_rows][n_cols] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

//Cách nối chân với NodeMCU
byte rowPins[n_rows] = {D7, D6, D5, D4};  //hàng
byte colPins[n_cols] = {D3, D2, D1, D0};  //cột

//cài đặt keypad
Keypad myKeypad = Keypad( makeKeymap(keys), rowPins, colPins, n_rows, n_cols); 
// cài đặt đèn LED
Adafruit_NeoPixel strip = Adafruit_NeoPixel(1, D8, NEO_GRB + NEO_KHZ800);

void setup() {
  //bật serial, baudrate 115200
  Serial.begin(115200);
  
  // Ket noi wifi
  connectWifi(); 

  // khởi động đèn LED
  strip.begin();  
}

void loop()
{
  // tạo biến myKey để lưu giá trị nút được bấm
  char myKey = myKeypad.getKey();

  // kiểm tra nếu có nút bấm
  if (myKey != NULL){
    // nếu số 1 màu vàng
    if (myKey == '1')
    {
      // cài đặt màu
      // thứ tự (đèn, mã R, G, B) (0,255,230,0);
      strip.setPixelColor(0,255,230,0);
      strip.show();
    }
    // nếu số 0 màu xanh
    else if (myKey == '0')
    {
      strip.setPixelColor(0,0,255,0);
      strip.show();
    }
    // còn lại màu đỏ
    else
    {
      strip.setPixelColor(0,255,0,0);
      strip.show();
    }
    // sẽ gửi dữ liệu lên server
    postServer(String (myKey));
    // hiện nút vừa bấm ở cổng serial
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
  String uri = "http://192.168.0.130:80/";
  // chuỗi data theo một quy định
  String data = "GET /?" "MACHINE_ID=" + MACHINE_ID + "&MACHINE_INDEX=" + MACHINE_INDEX + "&ERROR_INDEX=" + ERROR_DATA + " HTTP/1.0";
  // gửi request GET tới server (dưới đây là 1 chuỗi quy tắc GET):
  client.println(data);
  client.println();
  // hiện chuỗi request và kết quả ở cổng serial
  Serial.print("Requesting GET: ");
  Serial.println(data);
  Serial.println("Gui thanh cong");
}
