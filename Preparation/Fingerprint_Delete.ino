#include <Adafruit_Fingerprint.h>
#include <HardwareSerial.h>

// Use UART2: GPIO16 (RX), GPIO17 (TX)
HardwareSerial mySerial(2);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

void setup() {
  Serial.begin(115200);
  delay(1000);

  // Initialize fingerprint sensor serial
  mySerial.begin(57600, SERIAL_8N1, 16, 17); // RX=16, TX=17
  finger.begin(57600);

  // Verify sensor connection
  if (finger.verifyPassword()) {
    Serial.println("✅ R307 Fingerprint sensor detected.");
  } else {
    Serial.println("❌ Sensor not found or wrong password.");
    while (1);
  }

  // Delete all fingerprints
  Serial.println("Deleting all fingerprints...");
  uint8_t result = finger.emptyDatabase();

  if (result == FINGERPRINT_OK) {
    Serial.println("✅ All fingerprints deleted successfully.");
  } else {
    Serial.print("❌ Failed to delete fingerprints. Error code: ");
    Serial.println(result);
  }
}

void loop() {
  // Nothing to do here
}
