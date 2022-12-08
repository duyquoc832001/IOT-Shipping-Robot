////////////////Động cơ////////////////////////////////

//motor1 intializations
#define M1_A PA2
#define M1_B PA3
//motor2 intializations
#define M2_A PA0
#define M2_B PA1
//motor3 intializations
#define M3_A PB0
#define M3_B PB1
//motor4 intializations
#define M4_A PA8
#define M4_B PA11

//////////////////Encoder//////////////////////////////

//Encoder1 for motor1 intializations
#define encoder1PinA  PA6   //motor 1
#define encoder1PinB  PA7
//Encoder2 for motor1 intializations
#define encoder2PinA  PB8     //motor 2
#define encoder2PinB  PB9
//Encoder3 for motor1 intializations
#define encoder3PinA  PB11    //motor 3
#define encoder3PinB  PB10
//Encoder4 for motor1 intializations
#define encoder4PinA  PB3    //motor 4
#define encoder4PinB  PB4

//////////////////Cảm biến//////////////////////////////////
//CB Hành trình
#define CBHT PB15
//CB Siêu âm
// #define CBSA_ECHO PB14
// #define CBSA_TRIG PB13
#define CBSA_ECHO PB14

/////////////////////Button điều khiển/////////////////////
#define RE_LO PB12
#define AU_MA PA5

/////////////// Truyền dữ liệu từ ESP32 CAM /////////////////
#define START_STOP PA4
#define AUTO_MAN PB5
#define DES_1 PA15
#define DES_2 PA12
#define OBSTACLE PB13

