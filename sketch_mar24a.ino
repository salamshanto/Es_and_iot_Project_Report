#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define FLAME_SENSOR 34
#define BUZZER 25
#define LED 26
#define GSM_RX 16
#define GSM_TX 17

LiquidCrystal_I2C lcd(0x27, 16, 2);
bool fireAlertSent = false;

void setup() {
  pinMode(FLAME_SENSOR, INPUT);
  pinMode(LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, GSM_RX, GSM_TX);

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("System Ready");
  delay(2000);
  lcd.clear();
}

void loop() {
  int fireDetected = digitalRead(FLAME_SENSOR) == LOW;

  if (fireDetected) {
    lcd.setCursor(0, 0);
    lcd.print("Fire Detected! ");
    tone(BUZZER, 1000);

    blinkLED();  // Call LED blink function

    Serial.println("Fire detected! Buzzer ON");

    if (!fireAlertSent) {
      sendSMS("ALERT: FIRE detected! Call 01715930563.");
      fireAlertSent = true;
    }

  } else {
    lcd.setCursor(0, 0);
    lcd.print("Fire Normal     ");
    digitalWrite(LED, LOW);
    noTone(BUZZER);
    Serial.println("Fire normal! Buzzer OFF");
    fireAlertSent = false;
  }

  delay(1000);
}

void blinkLED() {
  for (int i = 0; i < 3; i++) {
    digitalWrite(LED, HIGH);
    delay(500);
    digitalWrite(LED, LOW);
    delay(1000);
  }
}

void sendSMS(String message) {
  Serial2.println("AT+CMGF=1");
  delay(1000);
  Serial2.println("AT+CMGS=\"01715930563\"");
  delay(1000);
  Serial2.print(message);
  delay(500);
  Serial2.write(26);
  delay(5000);
  Serial.println("SMS sent");
}
