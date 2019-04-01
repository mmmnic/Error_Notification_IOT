#include <Arduino_JSON.h>
#include <Servo.h>

int ledgreen = 0;
int ledred = 1;
int ledyellow = 2;
int servoPin = 9;
Servo s9;
String inputStr = "";
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  s9.attach(servoPin);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(ledgreen, OUTPUT);
  pinMode(ledred, OUTPUT);
  pinMode(ledyellow, OUTPUT);

  onLed(3);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available()){
    char character = Serial.read();
    inputStr += character;
    Serial.print(character);
    if (character == '}'){
      JSONVar myObject = JSON.parse(inputStr);
      Serial.println(inputStr);
      String command = (const char*) myObject["COMMAND"];
      Serial.println(command);
      if (command == "GREEN"){
        digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
        Serial.print("turn on led green");
        // Open Barrier
        onLed(0);
        s9.write(180);
        //delay(3000);
      }
      else if (command == "RED"){
        digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
        Serial.print("turn on led red");
        onLed(1);
      }
      else if (command == "YELLOW"){
        digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
        Serial.print("turn on led yellow");
        onLed(2);
        s9.write(0);
      }
      inputStr = "";
    }
  }
}

void onLed(int index){
  if (index == 0){
    digitalWrite(ledgreen, HIGH);
    digitalWrite(ledred, LOW);
    digitalWrite(ledyellow, LOW);
  }
  else if (index == 1){
    digitalWrite(ledgreen, LOW);
    digitalWrite(ledred, HIGH);
    digitalWrite(ledyellow, LOW);
  }
  else if (index == 2){
    digitalWrite(ledgreen, LOW);
    digitalWrite(ledred, LOW);
    digitalWrite(ledyellow, HIGH);
  }
}
