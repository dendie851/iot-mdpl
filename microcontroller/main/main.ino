#include <Wire.h>
#include <Adafruit_BMP280.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Ukuran Layar OLED (128x64 piksel)
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// Menggunakan Pin I2C Default ESP32 (SDA = 21, SCL = 22)
#define SDA_PIN 21
#define SCL_PIN 22

// Tekanan udara rata-rata tingkat laut standar (dalam hPa)
#define SEALEVELPRESSURE_HPA (1013.25)

// Inisialisasi Objek Sensor & Display
Adafruit_BMP280 bmp;
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup() {
  Serial.begin(115200);
  delay(1000);

  // Inisialisasi jalur I2C pada Pin 21 & 22
  Wire.begin(SDA_PIN, SCL_PIN);

  // Inisialisasi Layar OLED (Alamat I2C: 0x3C)
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("Gagal menemukan layar OLED!"));
    for(;;); 
  }

  // Inisialisasi Sensor BMP280 (Mencoba alamat 0x76, jika gagal coba 0x77)
  if (!bmp.begin(0x76)) {
    if (!bmp.begin(0x77)) {
      Serial.println(F("Gagal menemukan sensor BMP280! Cek kembali perkawatan kabelnya."));
      display.clearDisplay();
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(0, 20);
      display.println("BMP280 Error/Cek Kabel");
      display.display();
      for(;;); 
    }
  }

  /* Setting default sampling BMP280 untuk hasil pembacaan presisi */
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     
                  Adafruit_BMP280::SAMPLING_X2,     
                  Adafruit_BMP280::SAMPLING_X16,    
                  Adafruit_BMP280::FILTER_X16,      
                  Adafruit_BMP280::STANDBY_MS_500); 

  // Tampilan Awal / Splash Screen
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(15, 20);
  display.println("ALTITUDE METER");
  display.setCursor(25, 35);
  display.println("Inisialisasi...");
  display.display();
  delay(2000);
}

void loop() {
  // Pembacaan data dari sensor BMP280
  float temp = bmp.readTemperature();                       // Suhu (°C)
  float pressure = bmp.readPressure() / 100.0F;              // Tekanan (hPa)
  float altitude = bmp.readAltitude(SEALEVELPRESSURE_HPA);    // Ketinggian (MDPL)

  // Cetak ke Serial Monitor
  Serial.print("Suhu: "); Serial.print(temp); Serial.println(" *C");
  Serial.print("Tekanan: "); Serial.print(pressure); Serial.println(" hPa");
  Serial.print("Ketinggian: "); Serial.print(altitude); Serial.println(" m");
  Serial.println("--------------------------------");

  // Tampilkan Informasi pada Layar OLED
  display.clearDisplay();

  // Header
  display.setTextSize(1);
  display.setCursor(18, 0);
  display.println("ALTITUDE METER");
  display.drawLine(0, 10, 128, 10, WHITE); 

  // Nilai MDPL Utama
  display.setTextSize(2);
  display.setCursor(10, 20);
  display.print(altitude, 1); 
  display.setTextSize(1);
  display.println(" mdpl");

  // Garis pembatas bawah
  display.drawLine(0, 42, 128, 42, WHITE);

  // Informasi Tekanan Udara & Suhu
  display.setTextSize(1);
  display.setCursor(0, 48);
  display.print("Tek : ");
  display.print(pressure, 1);
  display.print(" hPa");

  display.setCursor(0, 56);
  display.print("Suhu: ");
  display.print(temp, 1);
  display.print(" C");

  // Perbarui tampilan OLED
  display.display();

  delay(1500);
}