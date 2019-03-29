/* 
 *  Analog cảm biến độ ẩm đất là cỡ 750, khi nhúng vào nước xuống còn 250
 *  
 */
#include <EEPROM.h>
#include <SoftwareSerial.h>
#include "DHT.h"  
//#include <Ticker.h>
#include <TinyGPS.h>
#include <LiquidCrystal_I2C.h>
#include <Arduino_JSON.h>

LiquidCrystal_I2C lcd(0x27,16,2);
/*
 * 0x27 là địa chỉ màn hình trong bus I2C. Phần này chúng ta không cần phải quá bận tâm vì hầu hết màn hình (20x4,...) đều như thế này!
 * 16 là số cột của màn hình (nếu dùng loại màn hình 20x4) thì thay bằng 20
 * 2 là số dòng của màn hình (nếu dùng loại màn hình 20x4) thì thay bằng 4
 */

TinyGPS gps;
SoftwareSerial ss(9, 8);    // RX(TX-GPS)   TX(RX_GPS)
SoftwareSerial loraSerial(5, 6); //TX(LORA), RX(LORA)

#define Button_increase 12 
#define Button_reduction 11
#define Button_lcd 10
#define Sensor_land A2
#define Pin_dht A3
#define Led 7  /* led bao hieu - relay */
#define Turn_on_GPS A0
#define Turn_off_GPS A1
#define M0 2
#define M1 3

const int DHT_type = DHT22;  //Khai báo loại cảm biến, có 2 loại là DHT11 và DHT22
DHT dht(Pin_dht, DHT_type);

int threshold = 0;    /* ------ đặt ngưỡng --------*/
int addr = 0;   /* địa chỉ lưu eeprom */
int temp_lcd = 0; 
float Longitude;  /* kinh độ */
float Latitude;   /* vĩ độ */
JSONVar myObject; // Khai báo Json Object

int temp_land;
bool Temp_GPS = false;
unsigned long lora_time = 0;  // dùng đề delay thời gian truyền lora

void Set_threshold();         /*---- hàm thiết đặt ngưỡng và lưu vào eeprom -------*/
void Display_LCD(float T, float H, int analog_land);     /*--- hàm hiển thị LCD ---*/
void Get_GPS();         /*------ Hàm lấy tọa độ kinh độ và vĩ độ -------*/

void setup() {
  Serial.begin(9600);
  //analogReference(DEFAULT);
  dht.begin();  // Khỏi động DHT22

  lcd.init();       //Khởi động màn hình. Bắt đầu cho phép Arduino sử dụng màn hình
  lcd.backlight();   //Bật đèn nền

  ss.begin(9600);   // khởi động GPS
  loraSerial.begin(9600);   // khởi động Lora

  pinMode(Button_increase,INPUT);
  pinMode(Button_reduction,INPUT);
  pinMode(Led,OUTPUT);
  pinMode(Sensor_land, INPUT);
  pinMode(M1, OUTPUT);
  pinMode(M0, OUTPUT);
  
  digitalWrite(M0, LOW);
  digitalWrite(M1, LOW);
  
  threshold = (EEPROM.read(addr)*1000) + (EEPROM.read(addr + 1)*100) + (EEPROM.read(addr + 2)*10) + (EEPROM.read(addr + 3)*1);  /* cập nhật lại giá trị khi mất điện */
}

void loop() {
/*----------- Đo nhiet độ, độ ẩm ------------*/
  Serial.println();
  float H = dht.readHumidity();    //Đọc độ ẩm
  float T = dht.readTemperature(); //Đọc nhiệt độ

  /*------------ Phần Lora ---------------*/
  if (lora_time == 0)
  {
    lora_time = millis();
    myObject["HUMIDITY"] = H;
    myObject["TEMPERATURE"] = T;
    myObject["NODEID"] = "NODE1";
    // JSON.stringify(myVar) dùng để đổi object ra string
    String jsonString = JSON.stringify(myObject);
    loraSerial.println(jsonString); // Send to Lora
    Serial.print("JSON.stringify(myObject) = ");
    Serial.println(jsonString);
  }
  if (millis() - lora_time >= 10000) // Đợi đủ 5s
  {
    lora_time = 0;
  }
  
  Serial.print("Nhiet do: ");
  Serial.println(T);               //Xuất nhiệt độ
  Serial.print("Do am: ");
  Serial.print(H);               //Xuất độ ẩm

/*---------- doc du lieu tu eeprom ----------*/
  Serial.println();
  Serial.print("EEPROM: ");
  Serial.print(EEPROM.read(addr));
  Serial.print("   ");
  Serial.print(EEPROM.read(addr + 1));
  Serial.print("   ");
  Serial.print(EEPROM.read(addr + 2));
  Serial.print("   ");
  Serial.print(EEPROM.read(addr + 3));
  Serial.println();
  
/*---------- đặt ngưỡng bằng button và lưu vào eeprom ----------*/
  Set_threshold();
/*------------- kiem tra nguong ---------------*/
  int analog_land = analogRead(Sensor_land);
  Serial.print("threshold: ");
  Serial.print(threshold);
  Serial.println();
  Serial.print("analog_land: ");
  Serial.print(analog_land);
  Serial.println();
  if (analog_land>=threshold)   /* do cam bien khi gặp nước trả về giá trị nhỏ hơn bình thường nên dùng <= */
  {
    digitalWrite(Led,HIGH);
    Serial.println("LED HIGH");

    /*
     *  xử lí relay bật
     *  digitalWrite(Relay,HIGH);
     */
  } else {
    digitalWrite(Led,LOW);
    Serial.println("LED LOW");  
    /*
     *  xử lí relay tắt
     *  digitalWrite(Relay,LOW);
     */
  }

/*----------- Lấy tọa độ GPS ---------------*/
  if (!digitalRead(Turn_on_GPS))
  {
    Temp_GPS = true;
  }
  if (!digitalRead(Turn_off_GPS))
  {
    Temp_GPS = false;
  }
  if (Temp_GPS)
  {
    Get_GPS();
  }
/*----------- Chuyển trang LCD -------------*/
  Display_LCD(T, H, analog_land);
/*-------------- ---------------*/
}




/*-------------- Hàm thiết đặt ngưỡng và lưu vào eeprom -----------------*/
void Set_threshold()
{
  if(!digitalRead(Button_increase))
  {
    while (!digitalRead(Button_increase));
    threshold = threshold + 10;
    int temp = threshold;
    EEPROM.write(addr, temp / 1000);  /*---- lưu số đầu tiên của threshold ---*/  
    temp = temp%1000;  
    EEPROM.write(addr + 1, temp / 100);  /*--- lưu số thứ 2 của threshold ---*/
    temp = temp%100;
    EEPROM.write(addr + 2, temp / 10);  /*--- lưu số thứ 3 của threshold ---*/
    EEPROM.write(addr + 3, temp % 10);  /*--- lưu số thứ 4 của threshold ---*/
  }
  if(!digitalRead(Button_reduction))
  {
    while (!digitalRead(Button_reduction));
    threshold = threshold - 10;
    int temp = threshold;
    EEPROM.write(addr, temp / 1000);  /*---- lưu số đầu tiên của threshold ---*/  
    temp = temp%1000;  
    EEPROM.write(addr + 1, temp / 100);  /*--- lưu số thứ 2 của threshold ---*/
    temp = temp%100;
    EEPROM.write(addr + 2, temp / 10);  /*--- lưu số thứ 3 của threshold ---*/
    EEPROM.write(addr + 3, temp % 10);  /*--- lưu số thứ 4 của threshold ---*/
  }
}


/*-------------- Hàm hiển thị trên LCD -----------------*/
void Display_LCD(float T, float H, int analog_land)
{
  if (!digitalRead(Button_lcd))
  {
    while(!digitalRead(Button_lcd));
    if(temp_lcd == 2)   /*--- xem nhiệt độ, độ ẩm ---*/
    {
      lcd.clear();
      temp_lcd = 0;
    } else {        /*--- xem ngưỡng ---*/
      lcd.clear();
      temp_lcd++;
    }
  }
  if(temp_lcd == 0)    /*--- xem nhiệt độ, độ ẩm ---*/
    {
      //lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Temp: ");
      lcd.print(T);
      lcd.setCursor(0,1);
      lcd.print("Humidity: ");
      lcd.print(H);
    } else {        /*--- xem ngưỡng ---*/
      if (temp_lcd == 1)
      {
        if (analog_land<1000 && temp_land>=1000)
        {
          lcd.clear();
          //lcd.setCursor(1,16);
          //lcd.print("F");
        }
        
        
        //lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Threshold: ");
        lcd.print(threshold);
        lcd.setCursor(0,1);
        lcd.print("Val_sensor: ");
        lcd.print(analog_land);
        
      } else {      /*----- xem vị trí node -----*/
        //lcd.clear();
        lcd.setCursor(0,1);
        if (Temp_GPS)
        {
          lcd.print("On ");
        } else {
          lcd.print("OFF");
        }
        lcd.setCursor(0,0);
        lcd.print("GPS:");
        lcd.print(Latitude,8);
        lcd.setCursor(4,1);
        lcd.print(Longitude,8);
      }      
    }
    temp_land = analog_land;
}

/*-------------- Hàm lấy tọa độ -------------*/
void Get_GPS()
{
  bool newData = false;
  // For one second we parse GPS data and report some key values
  for (unsigned long start = millis(); millis() - start < 1000;)
  {
    while (ss.available())
    {
      char c = ss.read();
      Serial.print(c);
      // Serial.write(c); // uncomment this line if you want to see the GPS data flowing
      if (gps.encode(c)){ // Did a new valid sentence come in?  
        newData = true;
      } 
    }    
  }
      
  if (newData)
  {
    float flat, flon;
    unsigned long age;
    gps.f_get_position(&flat, &flon, &age);
    //Serial.print("LAT=");
    //Serial.print(flat == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flat, 6);
    Latitude = flat == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flat, 6;      /*---- lấy vĩ độ -----*/
    //Serial.print(" LON=");
    //Serial.print(flon == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flon, 6);
    Longitude = flon == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flon, 6;     /*---- lấy kinh độ -----*/
    Serial.print("Kinh do: ");
    Serial.println(flon,6);
    Serial.print("Vi do: ");
    Serial.println(flat,6);
    
    //Serial.print(" SAT=");
    //Serial.print(gps.satellites() == TinyGPS::GPS_INVALID_SATELLITES ? 0 : gps.satellites());
    //Serial.print(" PREC=");
    //Serial.print(gps.hdop() == TinyGPS::GPS_INVALID_HDOP ? 0 : gps.hdop());
  }

}
