#include <SPI.h>
#include <MFRC522.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

#define SS_PIN 2  // Pin SDA ke GPIO2 (D4)
#define RST_PIN 0 // Pin RST ke GPIO0 (D3)
#define BUZZER_PIN 5 // Pin buzzer ke GPIO5 (D1)

const char* ssid = "yourssid";
const char* password = "yourpass";

MFRC522 rfid(SS_PIN, RST_PIN); // Inisialisasi instansi RFID

bool alreadyPresent = false; // Status absen

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
        WiFiClient client; // Objek WiFiClient
        String url = "http://192.168.238.23/absensi/getUser.php?uid=" + uidString; // Ganti dengan alamat IP server lokal Anda
        
        http.begin(client, url); // Pakai client
        int httpResponseCode = http.GET();

        if (httpResponseCode > 0) {
            String response = http.getString();
            Serial.println("Response dari server: " + response);

            // Buzzer sesuai input RFID
            if (response.indexOf("User not found") != -1) {
                // INnvalid Buzzer akan beep 3 kali
                for (int i = 0; i < 3; i++) {
                    tone(BUZZER_PIN, 5555);
                    delay(1000);
                    noTone(BUZZER_PIN);
                    delay(256);
                } // (broken)
                alreadyPresent = false; // Reset status absen
            } else if (response.indexOf("already present") != -1) {
                // Buzzer untuk sudah hadir
                for (int i = 0; i < 3; i++) {
                    tone(BUZZER_PIN, 500);
                    delay(500);
                    noTone(BUZZER_PIN);
                    delay(500);
                }  // (/broken)
            } else {
                // Valid Buzzer akan beep 2 kali
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
    delay(256); // Delay atau sleep setelah input kartu
}
