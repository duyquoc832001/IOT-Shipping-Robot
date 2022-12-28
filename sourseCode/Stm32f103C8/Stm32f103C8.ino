#include <mpu6050.h>
#include <Wire.h>
#include <PID_v1.h>
#include "motor.h"
#include "definePin.h"

MPU6050 mpu6050;

int stateLED = 0;

// Thiết lập thông số PID
double kp1 = 10, ki1 = 2, kd1 = 0.01, input1 = 0, output1 = 0, setpoint1 = 0;  // modify kp, ki and kd for optimal performance
double kp2 = 10, ki2 = 2, kd2 = 0.01, input2 = 0, output2 = 0, setpoint2 = 0;
double kp3 = 10, ki3 = 2, kd3 = 0.01, input3 = 0, output3 = 0, setpoint3 = 0;
double kp4 = 10, ki4 = 2, kd4 = 0.01, input4 = 0, output4 = 0, setpoint4 = 0;
float temp1;
float temp2;
float temp3;
float temp4;

volatile long precount1 = 0;
volatile long precount2 = 0;
volatile long precount3 = 0;
volatile long precount4 = 0;

float fb1 = 0;
float fb2 = 0;
float fb3 = 0;
float fb4 = 0;

PID M1PID(&input1, &output1, &setpoint1, kp1, ki1, kd1, DIRECT);
PID M2PID(&input2, &output2, &setpoint2, kp2, ki2, kd2, DIRECT);
PID M3PID(&input3, &output3, &setpoint3, kp3, ki3, kd3, DIRECT);
PID M4PID(&input4, &output4, &setpoint4, kp4, ki4, kd4, DIRECT);

// Thông số xe và các biến tính toán vị trí

const float n = 1.0416667;  // Tỉ lệ đọc encoder dựa vào thực tế
const float pi = 3.14;      // Số pi
const float R = 3;          //Bán kính bánh xe
const int N = 70;           // Số xung đếm được 1 vòng của encoder

volatile long encoderPos1 = 0;
volatile long encoderPos2 = 0;
volatile long encoderPos3 = 0;
volatile long encoderPos4 = 0;

float positionCurrent = 0;
float newPosition = 0;

// =======================

// Biến lưu trữ vị trí xe
int step = 0;
int step_CBSA = 0;
unsigned long timeCurrent = 0;
unsigned long time = 0;
unsigned long time_counter = 0;
unsigned long time_CBSA = 0;
unsigned long time_UART = 0;


// Thiết lập giá trị mặt định cho phương trình động học
float w1, w2, w3, w4;
float Vx = 0, Vy = 0, Wz = 0.0;
float lx = 0.10, ly = 0.6, r = 0.3;
float z;

int speed_setpoint = 22;
int rotate_setpoint = 35;
long restTime = 1500;
long position_des = 0;
long position_completion = 0;

// Thiết lập các biến cảm biến siêu âm
unsigned long time_SA;  //Thời gian là kiểu số nguyên
int distance_SA;        //Khoảng cách là kiểu số nguyên

// Thiết lập các giá trị mặc định giao tiếp uart

int start_stop = 0;     // biến chế độ hoạt động của chương trình
int selection_des = 2;  // chọn điểm đến gia công
int auto_man = 0;       // biến lựa chọn chế độ hoạt động (0: man, 1: auto)
int package = 0;        // biến xác nhận có hàng trên xe
int obstacle = 0;       // biến xác nhận có vật cản
int remote_local = 1;   // biến lựa chọn chế độ điều khiển (1: remote, 0: local)
int box_1 = 0;          // biến xác nhận có hàng tại station 1
int box_2 = 0;          // biến xác nhận có hàng tại station 2
int des_a = 0;          // biến xác nhận xe đã qua vị trí destination a
int des_b = 0;          // biến xác nhận xe đã qua vị trí destination b
int des_c = 0;          // biến xác nhận xe đã qua vị trí destination c
int s1 = 1;             // biến xác nhận xe đã qua vị trí station 1
int s2 = 0;             // biến xác nhận xe đã qua vị trí station 2

// the setup function runs once when you press reset or power the board
void setup() {
  // mpu mẫu
  Serial.begin(115200);
  Serial.println("\n\nWelcome to the MPU6050 basic example");
  Serial.println("Driver version " MPU6050_VERSION);
  Wire.begin();

  Serial.print("MPU6050: sensor is ... ");
  Serial.println(mpu6050.absent() ? "absent" : "present");

  int error = mpu6050.begin();
  Serial.print("MPU6050: ");
  Serial.println(mpu6050.error_str(error));
  delay(5000);
  // Thiết lập chân đọc xung encoder và ngắt
  pinMode(encoder1PinA, INPUT_PULLUP);
  pinMode(encoder1PinB, INPUT_PULLUP);
  pinMode(encoder2PinA, INPUT_PULLUP);
  pinMode(encoder2PinB, INPUT_PULLUP);
  pinMode(encoder3PinA, INPUT_PULLUP);
  pinMode(encoder3PinB, INPUT_PULLUP);
  pinMode(encoder4PinA, INPUT_PULLUP);
  pinMode(encoder4PinB, INPUT_PULLUP);

  // Setup các switch trên xe
  pinMode(CBHT, INPUT_PULLUP);
  pinMode(RE_LO, INPUT_PULLUP);
  pinMode(AU_MA, INPUT_PULLUP);

  // Setup các chân nhận dữ liệu từ esp
  pinMode(START_STOP, INPUT_PULLUP);
  pinMode(AUTO_MAN, INPUT_PULLUP);
  pinMode(DES_1, INPUT_PULLUP);
  pinMode(DES_2, INPUT_PULLUP);
  pinMode(OBSTACLE1, INPUT_PULLUP);


  attachInterrupt(digitalPinToInterrupt(encoder1PinA), encoder1, FALLING);
  attachInterrupt(digitalPinToInterrupt(encoder2PinA), encoder2, FALLING);
  attachInterrupt(digitalPinToInterrupt(encoder3PinA), encoder3, FALLING);
  attachInterrupt(digitalPinToInterrupt(encoder4PinA), encoder4, FALLING);

  //PID M1 intializations
  M1PID.SetMode(AUTOMATIC);
  M1PID.SetSampleTime(1);
  M1PID.SetOutputLimits(-255, 255);

  //PID M2 intializations
  M2PID.SetMode(AUTOMATIC);
  M2PID.SetSampleTime(1);
  M2PID.SetOutputLimits(-255, 255);

  //PID M3 intializations
  M3PID.SetMode(AUTOMATIC);
  M3PID.SetSampleTime(1);
  M3PID.SetOutputLimits(-255, 255);

  //PID M4 intializations
  M4PID.SetMode(AUTOMATIC);
  M4PID.SetSampleTime(1);
  M4PID.SetOutputLimits(-255, 255);

  // initialize digital pin LED_BUILTIN as an output.
  pinMode(M1_A, OUTPUT);
  pinMode(M1_B, OUTPUT);

  pinMode(M2_A, OUTPUT);
  pinMode(M2_B, OUTPUT);

  pinMode(M3_A, OUTPUT);
  pinMode(M3_B, OUTPUT);

  pinMode(M4_A, OUTPUT);
  pinMode(M4_B, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  timeCurrent = millis();
  z = mpu6050_yaw();
  // // Wz = -z * 2.0;
  Serial.print("z: ");
  Serial.println(z);

  Serial.print("step: ");
  Serial.println(step);

  // đọc giá trị của switch
  switch1();
  // Cài đặt thời gian đến các destination
  Selection_des();

  // Chế độ trên xe

  System();

  //phương trình động học  
  base1();

  // Chương trình điều khiển motor xe
  PID();

  //////////////////////////////////////////////////////
  // Gửi dữ liệu qua UART
  Send_Data();

  // Dừng xe
  if(Vx == 0 && Wz == 0) {
    stop();
  }
}

float mpu6050_yaw() {
  MPU6050_t data = mpu6050.get();
  while (data.dir.error != 0) {
    // I suffer from a lot of I2C problems
    Serial.println(mpu6050.error_str(data.dir.error));
    // Reset I2C
    // TWCR = 0; Wire.begin();
    // Reread
    data = mpu6050.get();
  }
  return data.dir.yaw;
}