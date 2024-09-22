#include <SPI.h>
#include <MFRC522.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

#define SS_PIN 2  // Pin SDA terhubung ke GPIO2 (D4)
#define RST_PIN 0 // Pin RST terhubung ke GPIO0 (D3)
#define BUZZER_PIN 5 // Pin buzzer terhubung ke GPIO5 (D1)

const char* ssid = "k1llv01d";
const char* password = "disgrace7k";

MFRC522 rfid(SS_PIN, RST_PIN); // Inisialisasi instance RFID

bool alreadyPresent = false; // Status kehadiran

void setup() {
    Serial.begin(115200);
    SPI.begin();
    rfid.PCD_Init();
    pinMode(BUZZER_PIN, OUTPUT);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("Terhubung ke WiFi!");
}

void loop() {
    if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()) {
        return;
    }

    String uidString = "";
    for (byte i = 0; i < rfid.uid.size; i++) {
        uidString += String(rfid.uid.uidByte[i], HEX);
    }

    Serial.println("UID dibaca: " + uidString);

    // Koneksi ke server PHP
    if (WiFi.status() == WL_CONNECTED) {
        HTTPClient http;
        WiFiClient client; // Buat objek WiFiClient
        String url = "http://192.168.238.23/absensi/getUser.php?uid=" + uidString; // Ganti dengan alamat IP server lokal Anda
        
        http.begin(client, url); // Menggunakan client
        int httpResponseCode = http.GET();

        if (httpResponseCode > 0) {
            String response = http.getString();
            Serial.println("Response dari server: " + response);

            // Aktifkan buzzer sesuai validasi
            if (response.indexOf("User not found") != -1) {
                // Buzzer untuk invalid (3 kali beep)
                for (int i = 0; i < 3; i++) {
                    tone(BUZZER_PIN, 5555);
                    delay(1000);
                    noTone(BUZZER_PIN);
                    delay(256);
                }
                alreadyPresent = false; // Reset status absen
            } else if (response.indexOf("already present") != -1) {
                // Buzzer untuk sudah hadir
                for (int i = 0; i < 3; i++) {
                    tone(BUZZER_PIN, 500);
                    delay(500);
                    noTone(BUZZER_PIN);
                    delay(500);
                }
            } else {
                // Buzzer untuk valid (2 kali beep)
                alreadyPresent = true; // Set status absen
                for (int i = 0; i < 2; i++) {
                    tone(BUZZER_PIN, 5555);
                    delay(50);
                    noTone(BUZZER_PIN);
                    delay(100);
                }
            }
        } else {
            Serial.print("Error on HTTP request: ");
            Serial.println(httpResponseCode);
        }
        http.end();
    }

    rfid.PICC_HaltA();
    delay(256); // Jeda sebelum membaca kartu berikutnya
}
