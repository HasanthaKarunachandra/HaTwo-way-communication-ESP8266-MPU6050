#include <ESP8266WiFi.h>
#include <ArduinoWebsockets.h>

#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

Adafruit_MPU6050 mpu;

using namespace websockets;
WebsocketsClient socket;
const char* websocketServer = "your local ip;
boolean connected = false;

const char* ssid="Your SSID";
const char* password="your wifi passward";

const int ledPin=2;
bool ledState =false;

const int buttonPin =0;

void setup() {
  Serial.begin(115200);

  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(ledPin,OUTPUT);
  digitalWrite(ledPin,LOW);

  connectWiFi();

  conectToWebSocket();

  socket.onMessage(handleMessage);
  socket.onEvent(handleEvent);

  initializeMPU6050();


}

void loop() {
  if(!connected){
    Serial.println("Connecting to WebSocket server");
    conectToWebSocket();
  }
  socket.poll();

  String dataMPU6050=readingMPU6050();
  socket.send(dataMPU6050+":MPU6050:localhost:esp");
  delay(1000);
  

  if(digitalRead(buttonPin== LOW)){
    delay(200);
    ledState = !ledState;
    digitalWrite(ledPin,ledState);
    socket.send(String(ledState)+ ":led:localhost:esp");
  }

}

void handleMessage(WebsocketsMessage message){
  Serial.println(message.data());
  String data =message.data();

  String status=parseData(data,1);
  String sensor =parseData(data,2);

  if(sensor == "led"){
    ledState =status.toInt();
    digitalWrite(ledPin,ledState);
  }
}


void handleEvent(WebsocketsEvent event,WSInterfaceString data){
  //lets implement

}

void conectToWebSocket(){
  connected = socket.connect(websocketServer);
  if(connected){
    Serial.println("Connected");
  }else{
    Serial.println("Connection failed");
  }

}

void connectWiFi() {
  WiFi.mode(WIFI_OFF);
  delay(1000);
  //This line hides the viewing of ESP as wifi hotspot
  WiFi.mode(WIFI_STA);
  
  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi");
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
    
  Serial.print("connected to : "); Serial.println(ssid);
  Serial.print("IP address: "); Serial.println(WiFi.localIP());
}

String parseData(String data, int index) {
  String result = "";
  int currentIndex = 1;
  int start = 0;
  int end = data.indexOf(":");

  while (end != -1 && currentIndex <= index) {
    if (currentIndex == index) {
      result = data.substring(start, end);
      break;
    }

    start = end + 1;
    end = data.indexOf(":", start);
    currentIndex++;
  }

  return result;
}

//MPU6050

void initializeMPU6050(){
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  Serial.print("Accelerometer range set to: ");
  switch (mpu.getAccelerometerRange()) {
  case MPU6050_RANGE_2_G:
    Serial.println("+-2G");
    break;
  case MPU6050_RANGE_4_G:
    Serial.println("+-4G");
    break;
  case MPU6050_RANGE_8_G:
    Serial.println("+-8G");
    break;
  case MPU6050_RANGE_16_G:
    Serial.println("+-16G");
    break;
  }
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  Serial.print("Gyro range set to: ");
  switch (mpu.getGyroRange()) {
  case MPU6050_RANGE_250_DEG:
    Serial.println("+- 250 deg/s");
    break;
  case MPU6050_RANGE_500_DEG:
    Serial.println("+- 500 deg/s");
    break;
  case MPU6050_RANGE_1000_DEG:
    Serial.println("+- 1000 deg/s");
    break;
  case MPU6050_RANGE_2000_DEG:
    Serial.println("+- 2000 deg/s");
    break;
  }

  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  Serial.print("Filter bandwidth set to: ");
  switch (mpu.getFilterBandwidth()) {
  case MPU6050_BAND_260_HZ:
    Serial.println("260 Hz");
    break;
  case MPU6050_BAND_184_HZ:
    Serial.println("184 Hz");
    break;
  case MPU6050_BAND_94_HZ:
    Serial.println("94 Hz");
    break;
  case MPU6050_BAND_44_HZ:
    Serial.println("44 Hz");
    break;
  case MPU6050_BAND_21_HZ:
    Serial.println("21 Hz");
    break;
  case MPU6050_BAND_10_HZ:
    Serial.println("10 Hz");
    break;
  case MPU6050_BAND_5_HZ:
    Serial.println("5 Hz");
    break;
  }

  Serial.println("");
  delay(100);
}

String readingMPU6050(){
  String resultMPU6050 = "";
   sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  /* Print out the values */
  Serial.print("Acceleration X: ");
  Serial.print(a.acceleration.x);
  Serial.print(", Y: ");
  Serial.print(a.acceleration.y);
  Serial.print(", Z: ");
  Serial.print(a.acceleration.z);
  Serial.println(" m/s^2");

  Serial.print("Rotation X: ");
  Serial.print(g.gyro.x);
  Serial.print(", Y: ");
  Serial.print(g.gyro.y);
  Serial.print(", Z: ");
  Serial.print(g.gyro.z);
  Serial.println(" rad/s");

  Serial.print("Temperature: ");
  Serial.print(temp.temperature);
  Serial.println(" degC");

  Serial.println("");
  delay(500);

  resultMPU6050=String(a.acceleration.x)+","+String(a.acceleration.y)+","+String(a.acceleration.z)+","+String(g.gyro.x)+","+String(g.gyro.y)+","+String(g.gyro.z)+","+String(temp.temperature);
  return resultMPU6050;
}