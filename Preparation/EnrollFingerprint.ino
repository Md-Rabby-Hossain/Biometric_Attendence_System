#include <Adafruit_Fingerprint.h>
#include <HardwareSerial.h>

// Use UART2: GPIO16 (RX), GPIO17 (TX)
HardwareSerial mySerial(2);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

int id = 1; // Start enrolling from ID 1

void setup() {
  Serial.begin(115200);
  delay(1000);

  mySerial.begin(57600, SERIAL_8N1, 16, 17);
  finger.begin(57600);

  if (finger.verifyPassword()) {
    Serial.println("R307 Fingerprint sensor detected.");
  } else {
    Serial.println("Sensor not found or wrong password.");
    while (1);
  }

  Serial.println("Ready to enroll...");
}

void loop() {
  Serial.print("Place finger to enroll as ID #");
  Serial.println(id);
  enrollFingerprint(id);
  id++; // move to next ID after each enrollment
  delay(1000);
}

void enrollFingerprint(uint8_t id) {
  int p = -1;
  Serial.println("Waiting for valid finger...");

  // Capture first image
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    if (p == FINGERPRINT_NOFINGER) {
      delay(50);
    } else if (p == FINGERPRINT_OK) {
      Serial.println("Image taken.");
      break;
    } else {
      Serial.println("Failed to capture image.");
      delay(50);
    }
  }

  // Convert image to template
  p = finger.image2Tz(1);
  if (p != FINGERPRINT_OK) {
    Serial.println("Image to template failed.");
    return;
  }

  Serial.println("Remove finger...");
  while (finger.getImage() != FINGERPRINT_NOFINGER) {
    delay(50);
  }

  Serial.println("Place same finger again...");
  while (finger.getImage() != FINGERPRINT_OK) {
    delay(50);
  }

  // Convert second image to template
  p = finger.image2Tz(2);
  if (p != FINGERPRINT_OK) {
    Serial.println("Image to template failed.");
    return;
  }

  // Create model
  p = finger.createModel();
  if (p != FINGERPRINT_OK) {
    Serial.println("Model creation failed.");
    return;
  }

  // Store model at given ID
  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK) {
    Serial.println("Fingerprint enrolled successfully.");
  } else {
    Serial.println("Failed to save fingerprint.");
  }

  Serial.println("Ready for next enrollment...");
}
