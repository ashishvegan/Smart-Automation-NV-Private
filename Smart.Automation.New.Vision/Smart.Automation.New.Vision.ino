#define RX 2
#define TX 3
#include<LiquidCrystal_I2C.h>
#include<SoftwareSerial.h>
LiquidCrystal_I2C lcd(0x3F, 16, 2);
SoftwareSerial master(RX, TX);
#define FLOWSENSORPIN 4 // INP

// Water Signal Data
volatile uint16_t pulses = 0;
volatile uint8_t lastflowpinstate;
volatile uint32_t lastflowratetimer = 0;
volatile float flowrate;
float litres = 0.0;

// Run-Time Data
String supplyStatus = "OFF";
String IoTStatus = "OFF";
String waterUsage;
String InternetSWStatus = "OFF", WaterSWStatus = "OFF";


// Hardware Pins
const int InternetSW = 5;
const int WaterSW = 6;
const int RedLed = 7, GreenLed = 8;

SIGNAL(TIMER0_COMPA_vect) {
  uint8_t x = digitalRead(FLOWSENSORPIN);
  if (x == lastflowpinstate) {
    lastflowratetimer++;
    return;
  }
  if (x == HIGH) {
    pulses++;
  }
  lastflowpinstate = x;
  flowrate = 1000.0;
  flowrate /= lastflowratetimer;
  lastflowratetimer = 0;
}
void useInterrupt(boolean v) {
  if (v) {
    OCR0A = 0xAF;
    TIMSK0 |= _BV(OCIE0A);
  } else {
    TIMSK0 &= ~_BV(OCIE0A);
  }
}

void setup()
{
  Serial.begin(9600);
  master.begin(9600);
  pinMode(InternetSW, INPUT); // 5
  pinMode(WaterSW, INPUT); // 6
  pinMode(RedLed, OUTPUT); // 7
  pinMode(GreenLed, OUTPUT); // 8
  lcd.init();
  lcd.backlight();
  pinMode(FLOWSENSORPIN, INPUT);
  digitalWrite(FLOWSENSORPIN, HIGH);
  lastflowpinstate = digitalRead(FLOWSENSORPIN);
  useInterrupt(true);
  lcdData("SMART AUTOMATION", "IoT PoC");
  delay(2000);
  lcdData("- TEAM MICROSOFT", "NewVision, Pune");
  delay(2000);
}
void loop() {
  // Water Reader Code
  litres = pulses;
  litres /= 7.5;
  litres /= 60.0;
  Serial.println(litres);
  waterUsage = (String)litres;

  if (digitalRead(WaterSW) == HIGH)
  {
    WaterSWStatus = "ON";
    digitalWrite(GreenLed, HIGH);
    digitalWrite(RedLed, LOW);
  }
  else
  {
    WaterSWStatus = "OFF";
    digitalWrite(GreenLed, LOW);
    digitalWrite(RedLed, HIGH);
  }
  if (digitalRead(InternetSW) == HIGH)
  {
    InternetSWStatus = "ON";
  }
  else
  {
    InternetSWStatus = "OFF";
  }
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("USAGE SUPPLY IoT");
  lcd.setCursor(0, 1);
  lcd.print(litres, 1);
  lcd.print("L");
  lcd.setCursor(8, 1);
  lcd.print(WaterSWStatus);
  lcd.setCursor(13, 1);
  lcd.print(InternetSWStatus);
  delay(2000);

  if (litres > 2.0)
  {
    Serial.println("API Call");
    master.print(litres);
    // RESET
    litres = 0;
    pulses = 0;
    lastflowratetimer = 0;
  }
  delay(1000);
}
void lcdData(String r1, String r2)
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(r1);
  lcd.setCursor(0, 1);
  lcd.print(r2);
}
