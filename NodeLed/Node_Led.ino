#include "ESP8266WiFi.h"
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <String.h>
#include <Ticker.h>

#define MR_1 D0             // CHAN RESET
#define SHCP_1 D1           // CHAN CLOCK
#define STCP_1 D2           // CHAN CHOT VA XUAT DATA
#define DS_1 D3             // CHAN INPUT DATA

#define MR_2 D5             // CHAN RESET 2
#define SHCP_2 D6           // CHAN CLOCK 2
#define STCP_2 D7           // CHAN CHOT 2
#define DS_2 D8             // CHAN INPUT DATA 2

const char* ssid = "HUAWEI";                  
const char* password = "12345678";
const char* host = "14.169.3.205";                // Host server

WiFiClient client;
const int httpPort = 80;

char MACHINE_ID[10];
char MACHINE_INDEX[10];
char ERROR_INDEX[10];

int dec=0;          // Lỗi máy
int id=0;           // id máy

int connect_ok = 1;
ESP8266WebServer server(80);
//Static IP address configuration
IPAddress staticIP(192, 168, 43, 175); //ESP static ip
IPAddress gateway(192, 168, 43, 116);   //IP Address of your WiFi Router (Gateway)
IPAddress subnet(255, 255, 255, 0);  //Subnet mask
IPAddress dns(8, 8, 8, 8);  //DNS


int Bin_3[4] = {0, 0, 0, 0};            // LỖI SẼ ĐƯỢC CHUYỂN THÀNH SỐ BINARY VÀ LƯU VÀO MẢNG - BIT CÓ TRỌNG SỐ CAO SẼ LƯU VÀO PHẦN TỬ CUỐI CỦA MẢNG
int Bin_2[4] = {0, 0, 0, 0};
int Bin_1[4] = {0, 0, 0, 0};

void Binary(int dec, int A[]); /*-------- Hàm đổi số ở dạng DEC sang dạng BIN và lưu vào mảng BIN ---------*/


void setup() {
  Serial.begin(9600);            
  Serial.println();
  
  WiFi.disconnect();
  WiFi.config(staticIP, subnet, gateway, dns);    // set ip cho node

  WiFi.begin(ssid, password);                   // kết nối wifi

  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED)         // đợi kết nối wifi
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

   pinMode(MR_1, OUTPUT);    /* khai báo 4 chân OUTPUT trong NodeMCU để điều khiển 4 chân INPUT IC HC595 SAU ĐÓ DỊCH VÀ ĐƯA VÀO IC 7447 */
   pinMode(SHCP_1, OUTPUT);
   pinMode(STCP_1, OUTPUT);
   pinMode(DS_1, OUTPUT);

   pinMode(MR_2, OUTPUT);    /* khai báo 4 chân OUTPUT trong NodeMCU để điều khiển 4 chân INPUT IC HC595 SAU ĐÓ DỊCH VÀ ĐƯA VÀO IC 7447 */
   pinMode(SHCP_2, OUTPUT);
   pinMode(STCP_2, OUTPUT);
   pinMode(DS_2, OUTPUT);

   digitalWrite(MR_1, HIGH);
   digitalWrite(SHCP_1, LOW);
   digitalWrite(STCP_1, LOW);
   digitalWrite(DS_1, LOW);

   digitalWrite(MR_2, HIGH);
   digitalWrite(SHCP_2, LOW);
   digitalWrite(STCP_2, LOW);
   digitalWrite(DS_2, LOW);
}

void loop() {
  server.handleClient();
  //getDataStatus();
 
}

void getDataStatus(){                   // hàm lấy data từ server về - hàm này không dùng
  Serial.println("Go Here");
  connect_ok = 1;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    //delay(1000);
    connect_ok = 0;
//    return;
  }
  if (connect_ok == 1){
    String link = "http://14.169.42.202/error_noti/GetStatus.php";
    String link_data = "GET " + link + " HTTP/1.1\r\n";
//    client.print(link_data);
    client.print(String("GET ") + link + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "User-Agent: BuildFailureDetectorESP8266\r\n" +
               "Connection: close\r\n\r\n");
    while (client.connected() || client.available())
    {
      if (client.available())
      {
        String line = client.readStringUntil('\n');
        if (line[0] == '[')
          Serial.println(line);
      }
    }
    client.stop();
    Serial.println("\n[Disconnected]");
  }
  
}

void postToServer()             // hàm post lỗi lên server - lỗi nhận được từ các node 
{
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    //delay(1000);
    return;
  }
  // địa chỉ trang cần post
  String uri = "http://14.169.3.205/error_noti/ErrorNoti.php";             
  // chuỗi data sau khi đã lấy ra từ json
  String data = "MACHINE_ID=" + String(MACHINE_ID) + "&MACHINE_INDEX=" + String(MACHINE_INDEX) + "&ERROR_INDEX=" + String(ERROR_INDEX);         // định dạng chuỗi để 
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
  server.arg("MACHINE_ID").toCharArray(MACHINE_ID, server.arg("MACHINE_ID").length()+1);                  // nhận MACHINE_ID từ node
  server.arg("MACHINE_INDEX").toCharArray(MACHINE_INDEX, server.arg("MACHINE_INDEX").length()+1);         // nhận MACHINE_IND từ node
  server.arg("ERROR_INDEX").toCharArray(ERROR_INDEX, server.arg("ERROR_INDEX").length()+1);               // nhận ERROR_INDEX từ node
  //Serial.println(server.arg("ip").length());
  Serial.print("MACHINE_INDEX: ");
  Serial.println(MACHINE_INDEX);
  Serial.print("ERROR_INDEX: ");
  Serial.println(ERROR_INDEX);

  dec = server.arg("ERROR_INDEX").toInt();          // lỗi máy được lưu vào server.arg("ERROR_INDEX") nhưng dưới dạng chuỗi, dùng .toInt() để chuyển thành số int sau đó lưu vào dec để xuất ra led 7
  id = server.arg("MACHINE_ID").toInt();            // tương tự như lỗi máy thì cái này lấy id máy. 
  
  Dislay();                 // gọi hàm hiển thị
  
  postToServer();           // gọi hàm postToServer để post lỗi máy và id máy lên server

  /*dưới dây là 1 số quy tắc bao gồm cả html - tạo server của node*/
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

void Binary(int dec, int A[])         // hàm chuyển đổi giá trị DEC sang BINARY 
{
  /* kiểm tra id xem máy nào thì lưu lỗi vào mảng đó - tưởng ứng:
   *  Mảng Bin_1 là lưu số lỗi dạng Binary của máy 1
   *  Mảng Bin_2 là lưu số lỗi dạng Binary của máy 2
   *  Mảng Bin_3 là lưu số lỗi dạng Binary của máy 3
   */
    if (id == 1){
      Bin_1[0] = 0;       /* update lại giá trị 0 để không bị sai số trong mảng đã lưu trước */
      Bin_1[1] = 0;
      Bin_1[2] = 0;
      Bin_1[3] = 0;
      int i = 0;
      while (dec != 0)    /* bắt đầu vào while để chuyển thành số binary - bit có trọng số cao nhất được lưu vào phần tử cuối của mảng theo số lượng bit chia được*/
      { 
        Bin_1[i] = dec % 2;      /*------ chia lấy phần dư ------*/ 
        Serial.print("Gia tri thu " + String(i) + " la: ");
        Serial.print(Bin_1[i]);
        Serial.print(" : ");
        dec = dec/2;           /*------ cập nhật lại giá trị nguyên của số dec được truyền vào (chia lấy phần nguyên) ----------*/
        i++;
      }
    }
    if (id == 2){         /* tương tự như trên */
      Bin_2[0] = 0;       /* update lại giá trị 0 để không bị sai số trong mảng đã lưu trước */
      Bin_2[1] = 0;
      Bin_2[2] = 0;
      Bin_2[3] = 0;
      int i = 0;
      while (dec != 0)
      { 
        Bin_2[i] = dec % 2;      /*------ chia lấy phần dư ------*/ 
        Serial.print("Gia tri thu " + String(i) + " la: ");
        Serial.print(Bin_2[i]);
        Serial.print(" : ");
        dec = dec/2;           /*------ cập nhật lại giá trị nguyên của số dec được truyền vào (chia lấy phần nguyên) ----------*/
        i++;
      }
    }
    if (id == 3){         /* tương tự như trên */
      Bin_3[0] = 0;       /* update lại giá trị 0 để không bị sai số trong mảng đã lưu trước */
      Bin_3[1] = 0;
      Bin_3[2] = 0;
      Bin_3[3] = 0;
      int i = 0;
      while (dec != 0)
      { 
        Bin_3[i] = dec % 2;      /*------ chia lấy phần dư ------*/ 
        Serial.print("Gia tri thu " + String(i) + " la: ");
        Serial.print(Bin_3[i]);
        Serial.print(" : ");
        dec = dec/2;           /*------ cập nhật lại giá trị nguyên của số dec được truyền vào (chia lấy phần nguyên) ----------*/
        i++;
      }
    }
}

void Dislay()         // hàm này dùng để hiển thị số lỗi ra led 7
{

//  MR_1 D0             // CHAN RESET
//  SHCP_1 D1           // CHAN CLOCK
//  STCP_1 D2           // CHAN CHOT VA XUAT DATA
//  DS_1 D3             // CHAN INPUT DATA

  if (id !=0)
  {
    if (id == 1)
    {
      Binary(dec, Bin_1);    /// nếu máy 1 thì tham số truyền vào là số lỗi và mảng Bin_1
      Shift_bit();
    }
    if (id == 2)             
    {
      Binary(dec, Bin_2);     /// nếu máy 2 thì tham số truyền vào là số lỗi và mảng Bin_2
      Shift_bit();
    }
    if (id == 3)              
    {
      Binary(dec, Bin_3);     /// nếu máy 3 thì tham số truyền vào là số lỗi và mảng Bin_3
      Shift_bit();
    }
  }
} 
void Shift_bit()              // hàm này dùng để truyền input vào cho IC HC595
{
  if (id == 1 || id == 2)      // do có 3 led nên dùng 2 con ic HC595 để điều khiển - trong đó 1 con điều khiển 2 led và 1 con điều khiển 1 led - trường hợp này điều khiển 2 led
  {                             //  đưa số binary lỗi của máy 1 vào
    Serial.print("ID May: ");
    Serial.println(id);
    digitalWrite(MR_1, LOW);    // Kích ức thấp chân MR để đưa trạng thái của IC vê 0 hết
    digitalWrite(MR_1, HIGH);    // sau đó bật lên lại để bắt đầu truyền bit
    for (int i=3; i>=0; i--)
    {
      digitalWrite(DS_1, Bin_1[i]);   // Mảng Bin_[x] có 4 phần tử nên dùng vòng for 4 lần - và truyền giá trị binary vào cho chân DS - chân nhận input cho HC595
      digitalWrite(SHCP_1, HIGH);     // bật clock lên để xác nhận nhận data vào 
      digitalWrite(SHCP_1, LOW);      // tắt clock
    }
    for (int i=3; i>=0; i--)          // tương tự như trên để đưa số binary lỗi của máy 2 vào 
    {
      digitalWrite(DS_1, Bin_2[i]);   
      digitalWrite(SHCP_1, HIGH);
      digitalWrite(SHCP_1, LOW);
    }
    digitalWrite(STCP_1, HIGH);       // bật chân chốt dữ liệu để xuất số đó ra led 7
    digitalWrite(STCP_1, LOW);        // tắt chân chốt đi - không ảnh hưởng output
  }

  if (id == 3)              // trường hợp này điều khiển 1 led
  {                         // tương tự như trên để đưa số binary lỗi của máy 3 vào 
    Serial.print("ID May: ");
    Serial.println(id);
    digitalWrite(MR_2, LOW);
    digitalWrite(MR_2, HIGH);
    for (int i=3; i>=0; i--)
    {
      digitalWrite(DS_2, Bin_3[i]);
      digitalWrite(SHCP_2, HIGH);
      digitalWrite(SHCP_2, LOW);
    }
    digitalWrite(STCP_2, HIGH);       // bật chân chốt dữ liệu để xuất số đó ra led 7
    digitalWrite(STCP_2, LOW);        // tắt chân chốt đi - không ảnh hưởng output
  }
}
