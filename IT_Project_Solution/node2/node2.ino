/* 
 *  Analog cảm biến độ ẩm đất là cỡ 750, khi nhúng vào nước xuống còn 250
 *  
 */
#include <EEPROM.h>
#include <SoftwareSerial.h>
#include "DHT.h"  
#include <Wire.h> 
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


const byte DS1307 = 0x68;   /* Địa chỉ của DS1307 */
const byte NumberOfFields = 7;    /* Số byte dữ liệu sẽ đọc từ DS1307 */
 
/* khai báo các biến thời gian */
int second, minute, hour, day, wday, month, year;
unsigned long time_count;
 
#define Button_increase 12 
#define Button_reduction 11
#define Button_lcd 10
#define Sensor_PH A2
#define Pin_dht A3
#define Led 7  /* led bao hieu */
#define Turn_on_off_GPS A0
#define Turn_on_off_motor A1
#define M0 2
#define M1 3

const int DHT_type = DHT22;  //Khai báo loại cảm biến, có 2 loại là DHT11 và DHT22
DHT dht(Pin_dht, DHT_type);

long threshold = 0;    /* ------ đặt ngưỡng --------*/
int addr = 0;   /* địa chỉ lưu eeprom */
int temp_lcd = 0; 
float Longitude;  /* kinh độ */
float Latitude;   /* vĩ độ */
bool Temp_GPS = false;    /* biến tạm dùng để kiểm tra cách hiển thị trong lcd */
bool Temp_motor = false;  /* biến tạm dùng để kiểm tra cách hiển thị trong may bơm */
bool temp_pump = false;   /* biến tạm dùng để kiểm tra cách hiển thị trong máy bơm */
int temp_threshold = 0;   /* biến tạm dùng để tính và thời gian cần đặt máy bơm */

unsigned long begin_time = 0;        /* dùng để lấy thời gian bật */
unsigned long end_time = 0;           /* dùng để lấy thời gian kết thúc */

unsigned long lora_time = 0;  // dùng đề delay thời gian truyền lora

JSONVar myObject; // Khai báo Json Object

void Set_threshold();         /*---- hàm thiết đặt ngưỡng và lưu vào eeprom -------*/
void Display_LCD(float T, float H, int analog_ph);     /*--- hàm hiển thị LCD ---*/
void Get_GPS();         /*------ Hàm lấy tọa độ kinh độ và vĩ độ -------*/
/*------------- Hàm DS1307 -------------*/
void readDS1307();
/* Chuyển từ format BCD (Binary-Coded Decimal) sang Decimal */
int bcd2dec(byte num);
/* Chuyển từ Decimal sang BCD */
int dec2bcd(byte num);
void digitalClockDisplay();
void printDigits(int digits);
/* cài đặt thời gian cho DS1307 */
void setTime(byte hr, byte min, byte sec, byte wd, byte d, byte mth, byte yr);

void setup() {
  Serial.begin(9600);
  
  dht.begin();  // Khỏi động DHT22

  lcd.init();       //Khởi động màn hình. Bắt đầu cho phép Arduino sử dụng màn hình
  lcd.backlight();   //Bật đèn nền

  ss.begin(9600);   // khởi động GPS
  loraSerial.begin(9600);   // khởi động Lora

  pinMode(Button_increase,INPUT);
  pinMode(Button_reduction,INPUT);
  pinMode(Led,OUTPUT);
  pinMode(Sensor_PH, INPUT);
  pinMode(M1, OUTPUT);
  pinMode(M0, OUTPUT);

  digitalWrite(M0, LOW);
  digitalWrite(M1, LOW);

   Wire.begin();  /* bật DS1307 */
  /* cài đặt thời gian cho module */
  //setTime(9, 19, 30, 3, 5, 3, 19); /*--------- 17:26:30 Tuesday 26-02-2019     Sunday -> 1   Monday -> 2   Tuesday -> 3  Wednesday -> 4    Thursday -> 5    Friday -> 6    Saturday -> 7 --------*/
  /*
   * Câu lệnh setTime là câu lệnh dùng để ghi thời gian vào RTC DS1307
   * Nếu thời gian trong DS1307 chạy sai hay do hết pin (thay pin) 
   * Xử lí bằng cách bỏ comment câu lệnh trên (câu lệnh setTime), chỉnh lại đúng thời gian theo thứ tự giờ (chế độ 24h) - phút - giây - thứ - ngày - tháng - năm (lấy 2 số cuối của năm)
   * Nạp lại sau đó comment câu lệnh trên lại rồi nạp lại lần nữa.
   */
  threshold = (EEPROM.read(addr)*1000000) + (EEPROM.read(addr + 1)*100000) + (EEPROM.read(addr + 2)*10000) + (EEPROM.read(addr + 3)*1000) + (EEPROM.read(addr + 4)*100) + (EEPROM.read(addr + 5)*10) + (EEPROM.read(addr + 5)*1);  /* cập nhật lại giá trị khi mất điện */
  temp_threshold = threshold;
}

void loop() {
/*----------- DS1307 ------------------*/
/* Đọc dữ liệu của DS1307 */
  readDS1307();
  //time_count = millis();
  //Serial.println(time_count);
   /* Hiển thị thời gian ra Serial monitor */
  digitalClockDisplay();
  
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
    myObject["NODEID"] = "NODE2";
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
  Serial.println(H);               //Xuất độ ẩm

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
  Serial.print("  ");
  Serial.print(EEPROM.read(addr + 4));
  Serial.print("  ");
  Serial.print(EEPROM.read(addr + 5));
  Serial.print("  ");
  Serial.print(EEPROM.read(addr + 6));
  Serial.println();
  
/*---------- đặt ngưỡng bằng button và lưu vào eeprom ----------*/
  Set_threshold();

/*----------- Lấy tọa độ GPS ---------------*/
   if (!digitalRead(Turn_on_off_GPS))
  {
    if (!Temp_GPS)
    {
      Temp_GPS = true;
    }
    else {
      Temp_GPS = false;
    }
    
  }
  if (Temp_GPS)
  {
    Get_GPS();
  }
/*----------- Bật máy bơm ---------------*/
  if (!digitalRead(Turn_on_off_motor))
  {
    if (!Temp_motor && threshold > 0)
    {
      Temp_motor = true;
      begin_time = millis();
      
    }
    else {
      Temp_motor = false;
      begin_time = 0;
      end_time = 0;
    }
  }
  if (Temp_motor) 
  {
     end_time = millis();
  }
/*------------- kiem tra nguong ---------------*/
  int analog_ph = analogRead(Sensor_PH);
  Serial.print("Threshold: ");
  Serial.print(threshold);
  Serial.println();
  Serial.print("Analog_ph: ");
  Serial.print(analog_ph);
  Serial.println();
  if (Temp_motor && (end_time - begin_time >= threshold-500))   /* kiem tra giờ cài đặt và giờ hiện tại nếu bằng nhau thì bật nếu máy bơm được kích hoạt */
  {
    Serial.print("May bom duoc da tat ");
    Serial.println(end_time);
    digitalWrite(Led,LOW);
    Temp_motor = false;
    /*
     *  xử lí relay bật
     *  digitalWrite(Relay,HIGH);
     */
  }
  if (Temp_motor && (end_time - begin_time < threshold) && digitalRead(Led) == LOW)
  {
    digitalWrite(Led,HIGH);
    Serial.print("May bom duoc bat ");
    Serial.println(begin_time);
    /*
     *  xử lí relay tắt
     *  digitalWrite(Relay,LOW);
     */
  }
/*----------- Chuyển trang LCD -------------*/
  Display_LCD(T, H, analog_ph);
/*-------------- ---------------*/
}



/*-------------------------------------- Hàm thiết đặt ngưỡng và lưu vào eeprom ------------------------------------------------*/
void Set_threshold()
{
  if(!digitalRead(Button_increase))
  {
    while (!digitalRead(Button_increase));
    threshold = threshold + 1000;       /* tính theo hàm millis nen 1000ms = 1s mỗi lần tăng */
    unsigned long temp = threshold;
    EEPROM.write(addr, temp / 1000000);  /*---- lưu số đầu tiên của threshold ---*/  
    temp = temp%1000000;  
    EEPROM.write(addr + 1, temp / 100000);  /*--- lưu số thứ 2 của threshold ---*/
    temp = temp%100000;
    EEPROM.write(addr + 2, temp / 10000);  /*--- lưu số thứ 3 của threshold ---*/
    temp = temp%10000;  
    EEPROM.write(addr + 3, temp / 1000);  /*--- lưu số thứ 4 của threshold ---*/
    temp = temp%1000;
    EEPROM.write(addr + 4, temp / 100);  /*--- lưu số thứ 5 của threshold ---*/
    temp = temp%100;
    EEPROM.write(addr + 5, temp / 10);  /*--- lưu số thứ 6 của threshold ---*/
    temp = temp%10;
    EEPROM.write(addr + 6, temp);       /*--- lưu số thứ 7 của threshold ---*/
  }
  if(!digitalRead(Button_reduction))
  {
    while (!digitalRead(Button_reduction));
    //threshold = 0;
    if (threshold - 1000 >=0)
    {
      threshold = threshold - 1000;   /* tính theo hàm millis nen 1000ms = 1s mỗi lần giam */
    } else {
      threshold = 0;                  /* nếu giảm quá 0 thì giá trị ngưỡng thời gian cài đặt sẽ là 0 */
    }
    unsigned long temp = threshold;
    EEPROM.write(addr, temp / 1000000);  /*---- lưu số đầu tiên của threshold ---*/  
    temp = temp%1000000;  
    EEPROM.write(addr + 1, temp / 100000);  /*--- lưu số thứ 2 của threshold ---*/
    temp = temp%100000;
    EEPROM.write(addr + 2, temp / 10000);  /*--- lưu số thứ 3 của threshold ---*/
    temp = temp%10000;  
    EEPROM.write(addr + 3, temp / 1000);  /*--- lưu số thứ 4 của threshold ---*/
    temp = temp%1000;
    EEPROM.write(addr + 4, temp / 100);  /*--- lưu số thứ 5 của threshold ---*/
    temp = temp%100;
    EEPROM.write(addr + 5, temp / 10);  /*--- lưu số thứ 6 của threshold ---*/
    temp = temp%10;
    EEPROM.write(addr + 6, temp);       /*--- lưu số thứ 7 của threshold ---*/
  }
}


/*----------------------------------------------------------------- Hàm hiển thị trên LCD -------------------------------------------------*/
/*
 * Menu:
 * Trang 0: Xem Giờ phút giây - ngày tháng năm
 * Trang 1: Xem Nhiệt độ - độ ẩm
 * Trang 2: Xem GPS 
 * Trang 3: Xem Giá trị PH - Set thời gian cài đặt
 * Trang 4: Xem Chế độ bật tắt máy bơm - thời gian bơm
 * 
 */
void Display_LCD(float T, float H, int analog_ph)
{
    if (!digitalRead(Button_lcd)) /*-------------- kiểm tra nút bấm lcd ----------------------*/
    {
      while(!digitalRead(Button_lcd));
      if(temp_lcd ==4 )   /*---------------- kiểm tra giá trị temp_lcd để có thể chuyển trang hợp lí -------------*/
      {
        lcd.clear();
        temp_lcd = 0;
      } else {        /*------------------ nếu hết trang xem thì quay về trang 1 ----------------*/
        lcd.clear();
        temp_lcd++;
      }
    }
   /*---------------------------- Hiển thị ngày và giờ -------------------------*/
   if (temp_lcd == 0)  
   {    
        /*------- Hiển thị giờ phút giây -------*/
        lcd.setCursor(0,0);
        lcd.print("Time: ");
        if (hour<10)
        {
           lcd.print("0");
           lcd.print(hour);
         } else {
              lcd.print(hour);
           }
           lcd.print(":");
           if (minute<10)
           {
              lcd.print("0");
              lcd.print(minute);
            } else {
              lcd.print(minute);
            }
            lcd.print(":");
            if (second<10)
            {
              lcd.print("0");
              lcd.print(second);
            } else {
              lcd.print(second);
            }
            /*------- Hiển thị ngày tháng năm -------*/
//            if (hour==23 && minute==59 && second == 60)
//            {
//              lcd.clear();
//            }
              lcd.setCursor(0,1);
              lcd.print("Date: ");
              if (day<10)
              {
                lcd.print("0");
                lcd.print(day);
              } else {
                lcd.print(day);
              }
              
              lcd.print("/");
              if (month<10)
              {
                lcd.print("0");
                lcd.print(month);
              } else {
                lcd.print(month);
              }
              lcd.print("/");
              lcd.print(year);
   }       
    /*---------------------------- xem nhiệt độ, độ ẩm ---------------------------*/
   if(temp_lcd == 1)    
   {
      //lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Temp: ");
      lcd.print(T);
      lcd.setCursor(0,1);
      lcd.print("Humidity: ");
      lcd.print(H);
   }
   /*------------------------- xem vị trí node ------------------------*/
   if (temp_lcd == 2)  
   {
      lcd.setCursor(0,1);
      if (Temp_GPS)
      {
         lcd.print("On ");
      } else {
         lcd.print("OFF");
        }
        //lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("GPS:");
        lcd.print(Latitude,8);
        lcd.setCursor(4,1);
        lcd.print(Longitude,8);
     } 
     /*------------------ hien thi thoi gian đặt máy bơm va gia tri ph --------------------*/
     if (temp_lcd==3)     
     {
        //lcd.clear();
        
        /* hien thi gia trị đo PH */
        lcd.setCursor(0,0);
        lcd.print("Val_PH: ");
        if (analog_ph < 1000 && analog_ph > 99 ){
          lcd.print(analog_ph);
          lcd.print(" ");
        } else {
          if (analog_ph < 100)
          {
            lcd.print(analog_ph);
            lcd.print("  ");
          } else { 
              lcd.print(analog_ph);
            }
        }
        /*------------- hien thi thoi gian --------------*/
        lcd.setCursor(0,1);     
        lcd.print("Set time:");
        if (threshold==0){
          lcd.print(threshold);
          lcd.print("      ");
        } else{
          if (threshold > 0 && threshold < 10000) {
            lcd.print(threshold);
            lcd.print("   ");
          } else {
              if (threshold > 9999 && threshold < 100000){
                lcd.print(threshold);
                lcd.print("  ");
            } else {
                if (threshold > 99999 && threshold < 1000000){
                  lcd.print(threshold);
                  lcd.print(" ");
                 } else {
                      if (threshold > 999999){
                        lcd.print(threshold);
                      }
                   }
                }
            }
        }
     } 
     /*-------------------------- hien thi trang thai may bom va thoi gian hen gio ------------------------------*/
     if (temp_lcd==4)   
     {
        if (Temp_motor)
        {
          lcd.setCursor(0,0);
          lcd.print("Pump status: ");
          lcd.print("ON ");
          lcd.setCursor(0,1);
          lcd.print("Countdown: ");
          if ((threshold-(end_time - begin_time))/1000>= 0 && (threshold-(end_time - begin_time))/1000 < 10)        /* hien thi thời gian cài đặt đếm ngược */
          {
            lcd.print((threshold-(end_time - begin_time))/1000);         /* vi khi cai đặt là cái mili giây nên khi hiển thị bằng giây thì chia 1000 */
            lcd.print("s   ");   /* s là thể hiện đơn vị giây */
          } else {
                if ((threshold-(end_time - begin_time))/1000>= 10 && (threshold-(end_time - begin_time))/1000 < 100)        /* hien thi thời gian cài đặt đếm ngược */
                {
                    lcd.print((threshold-(end_time - begin_time))/1000);        /* vi khi cai đặt là cái mili giây nên khi hiển thị bằng giây thì chia 1000 */
                    lcd.print("s  ");
                } else {
                    if ((threshold-(end_time - begin_time))/1000>= 100 && (threshold-(end_time - begin_time))/1000 < 1000)        /* hien thi thời gian cài đặt đếm ngược */
                    {
                        lcd.print((threshold-(end_time - begin_time))/1000);
                        lcd.print("s ");
                    } else {
                        if ((threshold-(end_time - begin_time))/1000>= 1000)        /* hien thi thời gian cài đặt đếm ngược */
                        {
                            lcd.print((threshold-(end_time - begin_time))/1000);        /* vi khi cai đặt là cái mili giây nên khi hiển thị bằng giây thì chia 1000 */
                            lcd.print("s");
                        }
                    }
                }
          }
          temp_pump = true;
        } else {
          if (temp_pump)
          {
            lcd.clear();
            temp_pump = false;
          }
          lcd.setCursor(0,0);
          lcd.print("Pump status: ");
          lcd.print("OFF");
        }
    }
         
}
/*------------------------------------------------------------ Các Hàm DS1307----------------------------------------------------*/
void readDS1307()
{
        Wire.beginTransmission(DS1307);
        Wire.write((byte)0x00);
        Wire.endTransmission();
        Wire.requestFrom(DS1307, NumberOfFields);
        
        second = bcd2dec(Wire.read() & 0x7f);
        minute = bcd2dec(Wire.read() );
        hour   = bcd2dec(Wire.read() & 0x3f); // chế độ 24h.
        wday   = bcd2dec(Wire.read() );
        day    = bcd2dec(Wire.read() );
        month  = bcd2dec(Wire.read() );
        year   = bcd2dec(Wire.read() );
        year += 2000;    
}
/*------------------ Chuyển từ format BCD (Binary-Coded Decimal) sang Decimal --------------------*/
int bcd2dec(byte num)
{
        return ((num/16 * 10) + (num % 16));
}
/*----------------------------------- Chuyển từ Decimal sang BCD ---------------------------------*/
int dec2bcd(byte num)
{
        return ((num/10 * 16) + (num % 10));
}
/*----------------------------------- Hàm in thời gian trên Serial -------------------------------*/
void digitalClockDisplay(){
    // digital clock display of the time
    Serial.print(hour);
    printDigits(minute);
    printDigits(second);
    Serial.print(" ");
    Serial.print(day);
    Serial.print(" ");
    Serial.print(month);
    Serial.print(" ");
    Serial.print(year); 
    Serial.println(); 
}
 
void printDigits(int digits){
    // các thành phần thời gian được ngăn chách bằng dấu :
    Serial.print(":");
        
    if(digits < 10)
        Serial.print('0');
    Serial.print(digits);
}
 
/*------------------------------------ cài đặt thời gian cho DS1307 ---------------------------------*/
void setTime(byte hr, byte min, byte sec, byte wd, byte d, byte mth, byte yr)
{
        Wire.beginTransmission(DS1307);
        Wire.write(byte(0x00)); // đặt lại pointer
        Wire.write(dec2bcd(sec));
        Wire.write(dec2bcd(min));
        Wire.write(dec2bcd(hr));
        Wire.write(dec2bcd(wd)); // day of week: Sunday = 1, Saturday = 7
        Wire.write(dec2bcd(d)); 
        Wire.write(dec2bcd(mth));
        Wire.write(dec2bcd(yr));
        Wire.endTransmission();
}

/*------------------------------------------------------------------ Hàm lấy tọa độ -------------------------------------------------------*/
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
