#include <WiFi.h>
#include <PubSubClient.h>
#include <SimpleTimer.h>

SimpleTimer timer;
int timerID1;

// Thiết lập giá trị test UART
String inputString = "";      // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete
#define LED 2

long last = 0;
String SendEsp = "";

// Thiết lập các giá trị mặc định

int start_stop = 0;     // biến chế độ hoạt động của chương trình
int selection_des = 1;  // chọn điểm đến gia công
int auto_man = 0;       // biến lựa chọn chế độ hoạt động
int package = 0;        // biến xác nhận có hàng trên xe
int remote_local = 0;   // biến lựa chọn chế độ điều khiển (0: local, 1: remote)
int box_1 = 0;          // biến xác nhận có hàng tại station 1
int box_2 = 0;          // biến xác nhận có hàng tại station 2
int des_a = 0;          // biến xác nhận xe đã qua vị trí destination a
int des_b = 0;          // biến xác nhận xe đã qua vị trí destination b
int des_c = 0;          // biến xác nhận xe đã qua vị trí destination c
int s1 = 0;             // biến xác nhận xe đã qua vị trí station 1
int s2 = 0;             // biến xác nhận xe đã qua vị trí station 2

char msg[50];
//int value = 0;
// WiFi
const char *ssid = "Quoc Khanh";    // Enter your WiFi name
const char *password = "qk200103";  // Enter WiFi password

// MQTT Broker
const char *mqtt_broker = "192.168.1.141";
const char *mqtt_username = "nhattri";
const char *mqtt_password = "nhattri";
const int mqtt_port = 1883;
WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  // setup UART
  Serial.begin(115200);
  // reserve 200 bytes for the inputString:
  inputString.reserve(200);
  pinMode(LED, OUTPUT);
  digitalWrite(LED, 0);
  last = millis();
  // Điều kiện ban đầu của các van


  timerID1 = timer.setInterval(250, Pub);

  // Set software serial baud to 115200;
  Serial.begin(115200);
  // connecting to a WiFi network
  WiFi.begin(ssid, password);
  // while (if)
  if (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");
  //connecting to a mqtt broker
  client.setServer(mqtt_broker, mqtt_port);
  client.setCallback(callback);
  while (!client.connected()) {
    String client_id = "esp8266-client-";
    client_id += String(WiFi.macAddress());
    Serial.printf("The client %s connects to the public mqtt broker\n", client_id.c_str());
    if (client.connect(client_id.c_str(), mqtt_username, mqtt_password)) {
      Serial.println("Public emqx mqtt broker connected");
    } else {
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);
    }
  }
  // publish and subscribe
  client.subscribe("start_stop");
  client.subscribe("auto_man");
  client.subscribe("destination");
  client.subscribe("uart");

  client.publish("state_robot", "0");
  client.publish("package", "0");
  client.publish("remote_local", "0");
  client.publish("state_destination", "1");

  // Package station
  client.publish("box_1", "0");
  client.publish("box_2", "0");

  // Position
  client.publish("des_a", "0");
  client.publish("des_b", "0");
  client.publish("des_c", "0");
  client.publish("s1", "0");
  client.publish("s2", "0");
}

void callback(String topic, byte *payload, unsigned int length) {
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
  Serial.print("Message:");
  String messageTemp;
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    messageTemp += (char)payload[i];
  }
  Serial.println();
  if (topic == "start_stop")
    start_stop = messageTemp.toInt();  //start_stop
  if (topic == "destination")
    selection_des = messageTemp.toInt();  //selection_des
  if (topic == "auto_man")
    auto_man = messageTemp.toInt();  //auto_man
}
void reconnect() {
  // Loop until we're reconnected (while/if)
  if (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.subscribe("start_stop");
      client.subscribe("auto_man");
      client.subscribe("destination");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 1 seconds");
      // Wait 5 seconds before retrying
      delay(1000);
    }
  }
}
void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  timer.run();
  // hiển thị cài đặt lên Serial
  // Serial.print("auto_man: ");
  // Serial.println(auto_man);
  // Serial.print("remote_local: ");
  // Serial.println(remote_local);
  // Serial.print("box_1: ");
  // Serial.println(box_1);
  // Serial.print("des_a: ");
  // Serial.println(des_a);
  // Serial.print("s1: ");
  // Serial.println(s1);
  Read_UART();
  // Send_Data();
}

void Send_Data() {
  if (millis() - last >= 2000) {
  last = millis();
  if (start_stop) {
    Serial.println("start_stop_true");
  } else {
    Serial.println("start_stop_false");
  }

  if (auto_man) {
    Serial.println("auto_man_true");
  } else {
    Serial.println("auto_man_false");
  }

  switch (selection_des) {
    case 1:
      Serial.println("state_destination_1");
      break;
    case 2:
      Serial.println("state_destination_2");
      break;
    case 3:
      Serial.println("state_destination_3");
      break;
    default:
      Serial.println("state_destination_1");
      break;
  }
  }
}

void Read_UART() {
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    inputString += inChar;

    if (inChar == '\n') {
      stringComplete = true;
    }
    if (stringComplete) {
      Serial.print("Data nhận: ");
      Serial.println(inputString);
      long inputSize = inputString.length() + 1;
      char mess[inputSize];
      inputString.toCharArray(mess, inputSize);
      client.publish("uart", mess);
      //============================
      // xử lý dữ liệu
      XuLyDuLieu(String(inputString));
      //============================
      inputString = "";
      stringComplete = false;
    }
  }
}

void XuLyDuLieu(String inputString) {
  if (inputString.indexOf("A0B") >= 0) {
    Serial.println("OFF thiết bị 1");
    digitalWrite(LED, 0);
  } else if (inputString.indexOf("A1B") >= 0) {
    Serial.println("ON thiết bị 1");
    digitalWrite(LED, 1);
  }
  if (inputString.indexOf("C0D") >= 0) {
    Serial.println("OFF thiết bị 2");
  } else if (inputString.indexOf("C1D") >= 0) {
    Serial.println("ON thiết bị 2");
  }

  // uart biến package
  if (inputString.indexOf("package_false") >= 0) {
    package = 0;
  } else if (inputString.indexOf("package_true") >= 0) {
    package = 1;
  }

  // uart biến remote_local
  if (inputString.indexOf("remote_local_false") >= 0) {
    remote_local = 0;
  } else if (inputString.indexOf("remote_local_true") >= 0) {
    remote_local = 1;
  }

  // uart biến box_1
  if (inputString.indexOf("box_1_false") >= 0) {
    box_1 = 0;
  } else if (inputString.indexOf("box_1_true") >= 0) {
    box_1 = 1;
  }

  // uart biến box_2
  if (inputString.indexOf("box_2_false") >= 0) {
    box_2 = 0;
  } else if (inputString.indexOf("box_2_true") >= 0) {
    box_2 = 1;
  }

  // uart biến des_a
  if (inputString.indexOf("des_a_false") >= 0) {
    des_a = 0;
  } else if (inputString.indexOf("des_a_true") >= 0) {
    des_a = 1;
  }

  // uart biến des_b
  if (inputString.indexOf("des_b_false") >= 0) {
    des_b = 0;
  } else if (inputString.indexOf("des_b_true") >= 0) {
    des_b = 1;
  }

  // uart biến des_c
  if (inputString.indexOf("des_c_false") >= 0) {
    des_c = 0;
  } else if (inputString.indexOf("des_c_true") >= 0) {
    des_c = 1;
  }

  // uart biến s1
  if (inputString.indexOf("s1_false") >= 0) {
    s1 = 0;
  } else if (inputString.indexOf("s1_true") >= 0) {
    s1 = 1;
  }

  // uart biến s2
  if (inputString.indexOf("s2_false") >= 0) {
    s2 = 0;
  } else if (inputString.indexOf("s2_true") >= 0) {
    s2 = 1;
  }
}

void Pub() {
  if (start_stop) {
    client.publish("state_robot", "1");
  } else {
    client.publish("state_robot", "0");
  }

  // Publish validate package
  if (package) {
    client.publish("package", "1");
  } else {
    client.publish("package", "0");
  }

  // Publish mode of control
  if (remote_local) {
    client.publish("remote_local", "1");
  } else {
    client.publish("remote_local", "0");
  }

  // Publish validate package at station
  if (box_1) {
    client.publish("box_1", "1");
  } else {
    client.publish("box_1", "0");
  }
  if (box_2) {
    client.publish("box_2", "1");
  } else {
    client.publish("box_2", "0");
  }

  // Publish position current
  if (des_a) {
    client.publish("des_a", "1");
  } else {
    client.publish("des_a", "0");
  }
  if (des_b) {
    client.publish("des_b", "1");
  } else {
    client.publish("des_b", "0");
  }
  if (des_c) {
    client.publish("des_c", "1");
  } else {
    client.publish("des_c", "0");
  }
  if (s1) {
    client.publish("s1", "1");
  } else {
    client.publish("s1", "0");
  }
  if (s2) {
    client.publish("s2", "1");
  } else {
    client.publish("s2", "0");
  }

  // Publish positon destination
  switch (selection_des) {
    case 1:
      client.publish("state_destination", "1");
      break;
    case 2:
      client.publish("state_destination", "2");
      break;
    case 3:
      client.publish("state_destination", "3");
      break;
    default:
      client.publish("state_destination", "1");
      break;
  }
}