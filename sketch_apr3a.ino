#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <AsyncWebSocket.h>

const char* ssid = "NCKH";
const char* password = "giangvien";

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

// Chân kết nối các động cơ
int ENA = 12;
int IN1 = 14;
int IN2 = 18;

int ENB = 25;
int IN3 = 27;
int IN4 = 22;

// Tốc độ mặc định của các động cơ
#define ENASpeed 180
#define ENBSpeed 180

// Các cảm biến dò line
int Sensor1 = 0;
int Sensor2 = 0;
int Sensor3 = 0;
int Sensor4 = 0;
int Sensor5 = 0;

void forward() {
  Serial.println("ok di thang");
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void backward() {
  Serial.println("ok di lui");
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void turn_left() {
  Serial.println("ok re trai");
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void turn_right() {
  Serial.println("ok re phai");
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void robot_stop() {
  Serial.println("ok dung");
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void detect_line() {
  Sensor1 = digitalRead(15);
  Sensor2 = digitalRead(17);
  Sensor3 = digitalRead(19);
  Sensor4 = digitalRead(21);
  Sensor5 = digitalRead(16);
  // Xử lý logic dò line
}


void onWebSocketEvent(AsyncWebSocket* server, AsyncWebSocketClient* client, AwsEventType type, void* arg, uint8_t* data, size_t len) {
  if (type == WS_EVT_CONNECT) {
    Serial.println("Websocket client connection received");
  } else if (type == WS_EVT_DISCONNECT) {
    Serial.println("Websocket client connection closed");
  } else if (type == WS_EVT_DATA) {
    // Dữ liệu được nhận từ WebSocket
    AwsFrameInfo* info = (AwsFrameInfo*)arg;
    if (info->opcode == WS_TEXT) {

      data[len] = 0;
      Serial.printf("Text message received: %s\n", (const char*)data);

      // Xử lý lệnh từ tin nhắn text
      String command = (const char*)data;
      if (command == "forward") {
        forward();
      } else if (command == "backward") {
        backward();
      } else if (command == "left") {
        turn_left();
      } else if (command == "right") {
        turn_right();
      } else if (command == "stop") {
        robot_stop();
      }
    }
  }
}

void setup() {
  Serial.begin(115200);

  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  pinMode(Sensor1, INPUT);
  pinMode(Sensor2, INPUT);
  pinMode(Sensor3, INPUT);
  pinMode(Sensor4, INPUT);
  pinMode(Sensor5, INPUT);

  analogWrite(ENA, ENASpeed);
  analogWrite(ENB, ENBSpeed);

  WiFi.softAP(ssid, password);

  // Lấy địa chỉ IP của điểm truy cập mềm
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi");

  // // In ra địa chỉ IP của ESP32
  // Serial.print("IP address: ");
  // Serial.println(WiFi.localIP());

  ws.onEvent(onWebSocketEvent);
  server.addHandler(&ws);

  server.begin();
}

void loop() {
  // detect_line();
}
