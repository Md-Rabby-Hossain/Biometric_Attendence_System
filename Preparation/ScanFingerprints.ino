#include <Adafruit_Fingerprint.h>
#include <HardwareSerial.h>

HardwareSerial mySerial(2);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

void setup() {
  Serial.begin(115200);
  delay(1000);

  mySerial.begin(57600, SERIAL_8N1, 16, 17);
  finger.begin(57600);

  if (finger.verifyPassword()) {
    Serial.println("R307 sensor ready.");
  } else {
    Serial.println("Sensor not found.");
    while (1);
  }

  Serial.println("Waiting for finger...");
}

void loop() {
  // Wait for a finger
  if (finger.getImage() != FINGERPRINT_OK) return;

  // Convert to template
  if (finger.image2Tz() != FINGERPRINT_OK) return;

  // Search database
  uint8_t p = finger.fingerSearch();

  if (p == FINGERPRINT_OK) {
    Serial.print("Match found! ID: ");
    Serial.print(finger.fingerID);
    Serial.print("  Confidence: ");
    Serial.println(finger.confidence);
  } else if (p == FINGERPRINT_NOTFOUND) {
    Serial.println("No match found.");
  } else {
    Serial.println("Error.");
  }

  // Now wait for finger to be removed before next scan
  while (finger.getImage() != FINGERPRINT_NOFINGER) {
    delay(100);
  }

  Serial.println("Remove finger...");
  delay(500); // a small delay before next loop
}
