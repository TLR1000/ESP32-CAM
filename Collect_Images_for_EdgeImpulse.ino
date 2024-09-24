//This version works unstable with Core V3.x

//https://circuitdigest.com/microcontroller-projects/object-recognition-using-esp32-cam-and-edge-impulse

/**
 * Collect images for Edge Impulse image
 * classification / object detection
 *
 * BE SURE TO SET "TOOLS > CORE DEBUG LEVEL = INFO"
 * to turn on debug messages
 */

// if you define WIFI_SSID and WIFI_PASS before importing the library,
// you can call connect() instead of connect(ssid, pass)
//
// If you set HOSTNAME and your router supports mDNS, you can access
// the camera at http://{HOSTNAME}.local

#define WIFI_SSID "Skynet"
#define WIFI_PASS "********"
#define HOSTNAME "esp32cam"


#include <eloquent_esp32cam.h>
#include <eloquent_esp32cam/extra/esp32/wifi/sta.h>
#include <eloquent_esp32cam/viz/image_collection.h>

using eloq::camera;
using eloq::wifi;
using eloq::viz::collectionServer;


void setup() {
  delay(3000);
  Serial.begin(115200);
  Serial.println("___IMAGE COLLECTION SERVER___");

  // camera settings
  // replace with your own model!
  camera.pinout.aithinker();  // Use this for AI Thinker model
  Serial.println("_Camera pinout set_");

  camera.brownout.disable();
  Serial.println("_Brownout protection disabled_");

  // Edge Impulse models work on square images
  // Use a predefined resolution
  camera.resolution.face();  // face resolution is 240x240
  Serial.println("_Resolution set to face (240x240)_");

  camera.quality.high();  // Set high quality
  Serial.println("_Quality set to high_");

  // init camera
  Serial.println("_Initializing camera_");
  while (!camera.begin().isOk())
    Serial.println(camera.exception.toString());

  // Retrieve and print the current resolution and quality
  sensor_t *s = esp_camera_sensor_get();  // Get the camera sensor object

  // Retrieve framesize (resolution index)
  int framesize = s->status.framesize;  // Get the current frame size (resolution index)
  Serial.print("Current framesize (index): ");
  Serial.println(framesize);

  // Retrieve quality
  int quality = s->status.quality;  // Get current JPEG quality
  Serial.print("Current quality (lower is better): ");
  Serial.println(quality);

  // connect to WiFi
  Serial.println("_Initializing wifi_");
  while (!wifi.connect().isOk()) {
    Serial.println(wifi.exception.toString());
    delay(500);  // Small delay to avoid spamming
  }

  // init face detection HTTP server
  Serial.println("_Initializing collection server_");
  while (!collectionServer.begin().isOk())
    Serial.println(collectionServer.exception.toString());

  Serial.println("Camera OK");
  Serial.println("WiFi OK");
  Serial.println("Image Collection Server OK");
  Serial.println(collectionServer.address());
}


void loop() {
  // server runs in a separate thread, no need to do anything here
}
