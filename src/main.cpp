#include "Arduino.h"
#include <LiquidCrystal_I2C.h>
#include "EEPROM.h"

LiquidCrystal_I2C lcd(0x27, 20, 4); // set the LCD address to 0x27 for a 16 chars and 2 line display
const int ledPin = LED_BUILTIN;     // LED built-in terhubung ke pin D4 (GPIO2) pada NodeMCU

// // variabel untuk parsing perintah serial
const byte numChars = 32;     // Panjang maksimal pesan yang diterima
char receivedChars[numChars]; // Buffer untuk menyimpan pesan yang diterima
bool newData = false;         // Menunjukkan apakah ada data baru yang diterima
char perintah_pc = '0';

// variabel umumls
int jumlah_qc = 0;
int jumlah_ok = 0;
int jumlah_ng = 0;
bool keadaan_barang;

void setup()
{
  Serial.begin(115200); // Memulai komunikasi serial pada baud rate 115200
  lcd.init();           // initialize the lcd
  // Print a message to the LCD.
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print("Auto QC Starting....");
  lcd.setCursor(0, 1);
  lcd.print("----            ----");
  lcd.setCursor(0, 2);
  lcd.print("----            ----");
  lcd.setCursor(0, 3);
  lcd.print("Powered By Python!");

  pinMode(ledPin, OUTPUT); // Mengatur pin LED sebagai output

  Serial.println("wemos d1 mini siap menerima perintah");
  delay(2000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Siap!!!");
}

void loop()
{

  // baca perintah pc dari serial komunikasi
  while (Serial.available() > 0)
  {
    char rc = Serial.read();
    rc = toUpperCase(rc);
    if (rc == 'N')
    {
      jumlah_qc++;
      jumlah_ok++;
      // lcd
      lcd.setCursor(0, 0);
      lcd.print("Barang NG      ");
      // Serial.println("pneumatic menyala");
      digitalWrite(ledPin, LOW); // led hidup
    }
    else if (rc == 'O')
    {
      jumlah_qc++;
      jumlah_ng++;
      // lcd
      lcd.setCursor(0, 0);
      lcd.print("Barang OK      ");
      // Serial.println("pneumatic mati");
      digitalWrite(ledPin, HIGH); // led mati
    }
  }

  // print lcd
  lcd.setCursor(0, 1);
  lcd.print("Total QC: ");
  lcd.print(jumlah_qc);
  lcd.setCursor(0, 2);
  lcd.print("Total NG: ");
  lcd.print(jumlah_ng);
  lcd.setCursor(0, 3);
  lcd.print("Total OK: ");
  lcd.print(jumlah_ok);

  // print serial
  Serial.print(" QC: ");
  Serial.print(jumlah_qc);
  Serial.print(" NG: ");
  Serial.print(jumlah_ng);
  Serial.print(" OK: ");
  Serial.println(jumlah_ok);
}
