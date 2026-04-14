#include <WiFi.h>
#include <WebServer.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_PCD8544.h>
#include <Wire.h>

// --- 1. Settings & Credentials ---
const char* ssid = "NewBestPhone";     // Replace with your WiFi SSID
const char* password = "sophiaiscool"; // Replace with your Password
const int buttonPin = 15;                // Button connected to GPIO 15

// --- 2. Hardware Objects ---
// Nokia 5110 Pins: CLK=18, DIN=23, DC=2, CE=5, RST=4
Adafruit_PCD8544 display = Adafruit_PCD8544(18, 23, 2, 5, 4);
Adafruit_MPU6050 mpu;
WebServer server(80);

// --- 3. Variables for Physics & ER2 Accuracy ---
float totalG = 0.0;
float maxG = 0.0;
float offsetG = 0.0;

// Function to create the webpage for your computer screen
String getHTML() {
  String html = "<!DOCTYPE html><html><head><meta http-equiv='refresh' content='1'>";
  html += "<style>body{font-family:Arial; text-align:center; background:#222; color:white;}";
  html += "h1{font-size:40px;} .val{font-size:100px; color:#00ff00;}</style></head><body>";
  html += "<h1>SPIN TEST LIVE</h1><div class='val'>" + String(totalG, 2) + " g</div>";
  html += "<h1>PEAK RECORDED</h1><div class='val' style='color:#ffcc00;'>" + String(maxG, 2) + " g</div>";
  html += "<p>Press physical button on device to reset peak</p></body></html>";
  return html;
}

void setup() {
  Serial.begin(115200);
  
  // Initialize Button with internal pull-up (Side B to GND)
  pinMode(buttonPin, INPUT_PULLUP);

  // Initialize Nokia Display
  display.begin();
  display.setContrast(55);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(BLACK);
  display.println("WIFI CONNECT...");
  display.display();

  // Start Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) { delay(500); }
  
  // Initialize MPU-6050
  if (!mpu.begin()) {
    display.clearDisplay();
    display.println("SENSOR ERROR");
    display.display();
    while (1) yield();
  }

  // Set Range to 4G to satisfy ER2 (measures up to 3g accurately)
  mpu.setAccelerometerRange(MPU6050_RANGE_4_G);

  // Setup Web Server Route
  server.on("/", []() {
    server.send(200, "text/html", getHTML());
  });
  server.begin();

  // Show IP Address on Nokia Screen
  display.clearDisplay();
  display.setCursor(0,0);
  display.println("SYSTEM READY");
  display.println("IP ADDRESS:");
  display.println(WiFi.localIP());
  display.display();
  delay(4000);
}

void loop() {
  server.handleClient(); // Refresh the web page for your PC
  
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  
  float gx = a.acceleration.x / 9.81;
  float gy = a.acceleration.y / 9.81;
  float gz = a.acceleration.z / 9.81;
  float currentRawG = sqrt(gx*gx + gy*gy + gz*gz);

  
  // We subtract our offset so the device reads 0.00 when at rest (minus gravity)
  totalG = currentRawG - 1.0 - offsetG; 

  
  if (totalG > maxG) {
    maxG = totalG;
  }

  
  // If button is pressed (LOW), reset peak and re-calibrate
  if (digitalRead(buttonPin) == LOW) {
    maxG = 0;
    offsetG = currentRawG - 1.0; 
    display.clearDisplay();
    display.setCursor(0,10);
    display.println("  RESETTING...  ");
    display.display();
    delay(1000); // Debounce and visual feedback
  }

   
  display.clearDisplay();
  
  // 1. Draw the top label
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("G-FORCE"); 

  // 2. Draw the HUGE Live Number (Middle)
  // TextSize 3 is about 24 pixels tall
  display.setTextSize(3);    
  display.setCursor(0, 12);  // Positioned just below the label
  display.print(totalG, 1);  // 1 decimal place makes it more stable to read

  // 3. Draw the Peak Record (Bottom)
  display.setTextSize(1);
  display.setCursor(0, 38);  // Put it at the very bottom edge
  display.print("PEAK: ");
  display.print(maxG, 2);    // 2 decimals here for accuracy
  
  display.display();

  delay(50); // High-speed sampling (20 times per second)
}
