#define S0 23
#define S1 22
#define S2 21
#define S3 19
#define sensorOut 18

int frequency = 0;
int red = 0;
int blue = 0;
int green = 0;

#include <Arduino.h>
#include <WiFi.h>
#include <FirebaseESP32.h>

// Provide the token generation process info.
#include <addons/TokenHelper.h>

// Provide the RTDB payload printing info and other helper functions.
#include <addons/RTDBHelper.h>

/* 1. Define the WiFi credentials */
#define WIFI_SSID "m"
#define WIFI_PASSWORD "11112222"

// For the following credentials, see examples/Authentications/SignInAsUser/EmailPassword/EmailPassword.ino

/* 2. Define the API Key */
#define API_KEY "AIzaSyDRZxXa-VNZ5dBAaiZcmHy42sZGvb4wn_Q"

/* 3. Define the RTDB URL */
#define DATABASE_URL "nyoba-atas-default-rtdb.firebaseio.com" //<databaseName>.firebaseio.com or <databaseName>.<region>.firebasedatabase.app

/* 4. Define the user Email and password that alreadey registerd or added in your project */
#define USER_EMAIL "hikmal.saidfar1@gmail.com"
#define USER_PASSWORD "hijikata678"

// Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;

unsigned long count = 0;

uint8_t i =0;

unsigned long previousTimeSensorWrite_Realtime = 0;
unsigned long SensorBlinkDelay_Realtime = 10;

unsigned long timeNow = millis();


void setup() {
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(sensorOut, INPUT);
  
  // Setting frequency-scaling to 20%
  digitalWrite(S0,HIGH);
  digitalWrite(S1,LOW);
  
  Serial.begin(115200);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD); //insialisasi WiFi
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) //menghubungkan ESP ke WiFi
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);

  /* Assign the api key (required) */
  config.api_key = API_KEY;

  /* Assign the user sign in credentials */
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;

  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; // see addons/TokenHelper.h

  // Comment or pass false value when WiFi reconnection will control by your code or third party library e.g. WiFiManager
  Firebase.reconnectNetwork(true);

  // Since v4.4.x, BearSSL engine was used, the SSL buffer need to be set.
  // Large data transmission may require larger RX buffer, otherwise connection issue or data read time out can be occurred.
  fbdo.setBSSLBufferSize(4096 /* Rx buffer size in bytes from 512 - 16384 */, 1024 /* Tx buffer size in bytes from 512 - 16384 */);

  Firebase.begin(&config, &auth);
}

void loop() {
  // Setting red filtered photodiodes to be read
  digitalWrite(S2,LOW);
  digitalWrite(S3,LOW);
  // Reading the output frequency
  frequency = pulseIn(sensorOut, LOW);
  red = pulseIn(sensorOut, LOW);
  // Printing the value on the serial monitor
  Serial.print("R= ");//printing name
  Serial.print(frequency);//printing RED color frequency
  Serial.print("  ");
  delay(100);

  // Setting Green filtered photodiodes to be read
  digitalWrite(S2,HIGH);
  digitalWrite(S3,HIGH);
  // Reading the output frequency
  frequency = pulseIn(sensorOut, LOW);
  green = pulseIn(sensorOut, LOW);
  // Printing the value on the serial monitor
  Serial.print("G= ");//printing name
  Serial.print(frequency);//printing RED color frequency
  Serial.print("  ");
  delay(100);

  // Setting Blue filtered photodiodes to be read
  digitalWrite(S2,LOW);
  digitalWrite(S3,HIGH);
  // Reading the output frequency
  frequency = pulseIn(sensorOut, LOW);
  blue = pulseIn(sensorOut, LOW);
  // Printing the value on the serial monitor
  Serial.print("B= ");//printing name
  Serial.print(frequency);//printing RED color frequency
  Serial.println("  ");
  delay(100);

  if(red > 150 && green > 250 && blue > 300)
  {
    Serial.print("Ijo");
    Serial.print("   ");
  }  
  if(red < 200 && green > 250 && blue > 230)
  {
    Serial.print("Merah");
    Serial.print("   ");
  }
  if(red < 250 && green > 250 && blue < 250)
  {
    Serial.print("Biru");
    Serial.print("   ");
  }

  if (timeNow - previousTimeSensorWrite_Realtime > SensorBlinkDelay_Realtime){
  previousTimeSensorWrite_Realtime += SensorBlinkDelay_Realtime;
  
  Serial.printf("Set int... %s\n", Firebase.setInt(fbdo, F("/test/color/red"), red) ? "ok" : fbdo.errorReason().c_str());
  Serial.printf("Get int... %s\n", Firebase.getInt(fbdo, F("/test/color/red")) ? String(fbdo.to<int>()).c_str() : fbdo.errorReason().c_str());  
  
  Serial.printf("Set int... %s\n", Firebase.setInt(fbdo, F("/test/color/green"), green) ? "ok" : fbdo.errorReason().c_str());
  Serial.printf("Get int... %s\n", Firebase.getInt(fbdo, F("/test/color/green")) ? String(fbdo.to<int>()).c_str() : fbdo.errorReason().c_str());  
  
  Serial.printf("Set int... %s\n", Firebase.setInt(fbdo, F("/test/color/blue"), blue) ? "ok" : fbdo.errorReason().c_str());
  Serial.printf("Get int... %s\n", Firebase.getInt(fbdo, F("/test/color/blue")) ? String(fbdo.to<int>()).c_str() : fbdo.errorReason().c_str());  
  }
}