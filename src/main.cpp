#include <Arduino.h>
#include <WiFiNINA.h>
#include <ArduinoIoTCloud.h>
#include <Arduino_ConnectionHandler.h>

// Define the WiFi credentials
#define ssid "Wokwi-GUEST"
#define password ""

// Define the device login credentials
#define DEVICE_LOGIN_NAME ""
#define DEVICE_KEY ""

#define LEDPIN_WIFI 2
#define LEDPIN_IOT 0

bool IOT_BULB_STATE = false;

// Function to handle the change in IoT bulb state
void onBulbStateChange() {
  digitalWrite(LEDPIN_IOT, IOT_BULB_STATE ? HIGH : LOW); // Turn on/off the IoT LED
  Serial.println(IOT_BULB_STATE ? "Bulb ON" : "Bulb OFF");
}

void initWiFi() {
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi ..");

  // Blink the LED while connecting
  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(LEDPIN_WIFI, HIGH);
    delay(500);
    digitalWrite(LEDPIN_WIFI, LOW);
    delay(500);
    Serial.print(".");
  }

  Serial.println("Connected");
  Serial.println(WiFi.status());
  Serial.println(WiFi.localIP());
  Serial.print("RSSI: ");
  Serial.println(WiFi.RSSI());

  // Turn on the LED after successful connection
  digitalWrite(LEDPIN_WIFI, HIGH);
}

// Initialize IoT connection
void initIOT() {
  ArduinoCloud.setBoardId(DEVICE_LOGIN_NAME);
  ArduinoCloud.setSecretDeviceKey(DEVICE_KEY);

  // Link the cloud property with the callback
  ArduinoCloud.addProperty(IOT_BULB_STATE, READWRITE, ON_CHANGE, onBulbStateChange);

  // Start the cloud connection
  WiFiClient wifiClient;
  ArduinoCloud.begin(wifiClient);

  Serial.println("Connected to Arduino IoT Cloud");
  digitalWrite(LEDPIN_IOT, HIGH);  // Indicate successful IoT connection
}

void setup() {
  Serial.begin(115200);
  pinMode(LEDPIN_WIFI, OUTPUT);  // Set WiFi LED pin as output
  pinMode(LEDPIN_IOT, OUTPUT);   // Set IoT LED pin as output

  initWiFi();  // Connect to WiFi
  initIOT();   // Initialize IoT Cloud connection
}

void loop() {
   ArduinoCloud.update();  // Keep the IoT Cloud connection alive
}
