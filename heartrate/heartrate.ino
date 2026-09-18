/*
 * heartrate.ino  —  Arduino IDE sürümü
 *
 * MAX30102'den kalp atışı (PPG) örneklerini okuyup USART0 üzerinden
 * ham sayı olarak Serial Plotter'a gönderir. Grafiği Serial Plotter
 * otomatik çizer; burada ayrı bir çizim kodu yoktur.
 *
 * Kart            : Arduino Mega 2560 (ATmega2560), 16 MHz
 * Serial Plotter  : 9600 baud
 *
 * NEDEN extern "C" ?
 *   i2c.c, usart0.c ve max30102.c birer C dosyasıdır; bu .ino ise C++
 *   olarak derlenir. C++ tarafı, C fonksiyonlarını "extern C" bildirimi
 *   olmadan bulamaz ve bağlama (link) aşamasında "undefined reference"
 *   hatası verir. Başlıkları bu blok içinde dahil etmek bunu çözer.
 *
 * BAŞLIK SIRASI:
 *   i2c.h ve usart0.h "uint8_t" kullanır ama kendileri <avr/io.h> dahil
 *   etmez. <avr/io.h>'yi getiren max30102.h'yi önce dahil ediyoruz.
 *
 * NOT: Bu sketch içinde Serial.begin()/Serial.print() KULLANMA. USART0
 *   donanımını senin usart0.c dosyan ayarlıyor; ikisi aynı donanımı
 *   paylaştığı için karışmamaları gerekir.
 */

extern "C" {
  #include "max30102.h"   // <avr/io.h> -> uint8_t buradan gelir
  #include "i2c.h"
  #include "usart0.h"
}

void setup() {
  init_I2C();        // TWI / I2C başlat (~10 kHz)
  init_USART0();     // USART0 başlat (9600 baud, 8N1)
  delay(1000);       // güç ve çevrebirim oturma süresi
  MAX30102init();    // sensörü Heart-Rate (sadece RED) moduna al, başlat
  delay(1000);       // ilk geçerli örnekler için kısa stabilizasyon
}

void loop() {
  MAX30102read();    // PPG_RDY geldiyse 1 örnek oku, Serial Plotter'a yolla
}
