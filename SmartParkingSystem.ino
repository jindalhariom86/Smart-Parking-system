#define BLYNK_TEMPLATE_ID "TMPL3-MqJAZC4"
#define BLYNK_TEMPLATE_NAME "Smart Parking System"
#define BLYNK_AUTH_TOKEN "b3gGf5fB_vDQZYAwudu8UeoNmGArHdx6"

#include <ESP32Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

const int IRSensorPinEntry = 5;
// const int IRSensorPin1 = 13;
// const int IRSensorPin2 = 2;
// const int IRSensorPin3 = 14;
// const int IRSensorPin4 = 4;
const int IRSensorPinExit = 12;
const int servoPin = 15;
const int Total_Slots = 4;
Servo gateServo;
bool entryGateOpened = false;
bool exitGateOpened = false;

LiquidCrystal_I2C lcd(0x27, 16, 2);

int SlotPin[Total_Slots] = { 13, 16, 14, 4 };
int previousSlot = -1;

const char* ssid = "Hariom_wifi";
const char* password = "12345678";

void setup() {
  // put your setup code here, to run once:

  for (int i = 0; i < Total_Slots; i++) {
    pinMode(SlotPin[i], INPUT);
  }
  pinMode(IRSensorPinEntry, INPUT);
  // pinMode(IRSensorPin1, INPUT);
  // pinMode(IRSensorPin2, INPUT);
  // pinMode(IRSensorPin3, INPUT);
  // pinMode(IRSensorPin4, INPUT);
  pinMode(IRSensorPinExit, INPUT);
  Serial.begin(115200);
  gateServo.attach(servoPin);
  lcd.init();
  lcd.backlight();
  // WiFi.begin(ssid, password);

  // Serial.print("Connecting");

  Serial.println("Connecting to Blynk...");
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, password);
  Serial.println("Connected!");

  // Serial.println();
  // Serial.println("IP Address :  ");
  // Serial.println(WiFi.localIP());
}

void loop() {
  // put your main code here, to run repeatedly:
  Blynk.run();

  int IRSensorEntryValue = digitalRead(IRSensorPinEntry);
  int IRSensorExitValue = digitalRead(IRSensorPinExit);
  int availableSlots = 0;
  int occupiedSlots = 0;

  if (entryGateOpened == false) {

    if (IRSensorEntryValue == HIGH) {

      gateServo.write(90);
      entryGateOpened = true;
      Blynk.virtualWrite(V2, "OPEN");
      delay(3000);

    }

    else {

      gateServo.write(0);
    }
  }

  else {
    gateServo.write(0);
    entryGateOpened = false;
    Blynk.virtualWrite(V2, "CLOSED");
  }

  // Slots count

  for (int i = 0; i < Total_Slots; i++) {
    occupiedSlots += digitalRead(SlotPin[i]);
  }

  Blynk.virtualWrite(V4, digitalRead(SlotPin[0]));
  Blynk.virtualWrite(V5, digitalRead(SlotPin[1]));
  Blynk.virtualWrite(V6, digitalRead(SlotPin[2]));
  Blynk.virtualWrite(V7, digitalRead(SlotPin[3]));
  availableSlots = Total_Slots - occupiedSlots;

  // occupiedSlots = int(digitalRead(IRSensorPin1)) + int(digitalRead(IRSensorPin2)) + int(digitalRead(IRSensorPin3)) + int(digitalRead(IRSensorPin4));
  // availableSlots = Total_Slots - occupiedSlots;

  Serial.print("Occupied: ");
  Serial.println(occupiedSlots);

  Serial.print("Available: ");
  Serial.println(availableSlots);

  Blynk.virtualWrite(V0, availableSlots);
  Blynk.virtualWrite(V1, occupiedSlots);

  if (availableSlots != previousSlot) {
    if (availableSlots == 0) {

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Smart Parking");
      lcd.setCursor(0, 1);
      lcd.print("Parking Full!!");
      Blynk.virtualWrite(V3, "FULL");

    }

    else if (availableSlots <= 1) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Smart Parking");
      lcd.setCursor(0, 1);
      lcd.print("Almost Full!!");
      Blynk.virtualWrite(V3, "AVAILABLE");
    }

    else {
      lcd.clear();

      lcd.setCursor(0, 0);
      lcd.print("Smart Parking");

      lcd.setCursor(0, 1);
      lcd.print("Slots : ");
      lcd.print(availableSlots);
      Blynk.virtualWrite(V3, "AVAILABLE");
    }
    previousSlot = availableSlots;
  }

  delay(500);

  // if (exitGateOpened == false) {

  //   if (IRSensorExitValue == HIGH) {
  //     gateServo.write(90);
  //     exitGateOpened = true;
  //   } else {
  //     gateServo.write(0);
  //   }
  // } else {
  //   gateServo.write(0);
  //   exitGateOpened = false;
  // }
  // delay(500);
}
