#include <Arduino.h>
#include <string.h>

// Coin vibrator Motor OutPut through optocoupler with 5V output:
//PWM 325/1024 -> 81/255 = 1V
//PWM 1024/1024 -> 255/255 = 3.3V

// Declaration of the functions defined later
void change_mode();
void run_it();
void run_mode_0();
void run_mode_1();
void run_mode_2();
void run_mode_3();
void run_mode_4();
void run_mode_5();
void run_mode_6();
void display_mode(int num);
void activate(int* pins, int pwm);
void deactivate(int* pins);
void motor_activation_proportionnal_to_push();
void snooze(int time_between);

void test_LED();
void test_motors();
void test_piezo();
void test_button();


// Global variables definition
int activation_mode = 0;
float speed;
bool side = 0;
const int MAX_MODE = 6; //Defines the MAX number of run_mode() fct are used
int steps = 0;

//INPUT PINS
const int PIN_INPUT_BUTTON = 18;
//const uint8_t PIN_INPUT_POTENTIOMETER = A0;  // Deprecated
const uint8_t PIN_INPUT_PIEZO_LEFT_PUSH = A2;
const uint8_t PIN_INPUT_PIEZO_LEFT_RELEASE = A1;
const uint8_t PIN_INPUT_PIEZO_RIGHT_PUSH = A3;
const uint8_t PIN_INPUT_PIEZO_RIGHT_RELEASE = A4;

// BUTTON OUTPUT PIN
const int PIN_OUTPUT_BUTTON = 13;

// MOTOR OUTPUT PINS
const int PIN_MOTOR_L1 = 2;
const int PIN_MOTOR_L2 = 3;
const int PIN_MOTOR_L3 = 4;
const int PIN_MOTOR_R1 = 5;
const int PIN_MOTOR_R2 = 6;
const int PIN_MOTOR_R3 = 7;

// 7 DIGIT LED pins
const int PIN_LED_A = 26;
const int PIN_LED_B = 27;
const int PIN_LED_C = 22;
const int PIN_LED_D = 45;
const int PIN_LED_E = 44;
const int PIN_LED_F = 23;
const int PIN_LED_G = 51;
const int PIN_LED_DP = 50;
const int SLOT_1;                                 //>>>>>>>>>>>>>>>>>>>>> TO DEFINE
const int SLOT_2;                                 //>>>>>>>>>>>>>>>>>>>>> TO DEFINE
const int SLOT_3;                                 //>>>>>>>>>>>>>>>>>>>>> TO DEFINE
const int SLOT_4;                                 //>>>>>>>>>>>>>>>>>>>>> TO DEFINE

// Organisation in list of the PINs settings for use in later defined functions
int pins_left [4] = {PIN_MOTOR_L1, PIN_MOTOR_L2, PIN_MOTOR_L3};
int pins_right [4] = {PIN_MOTOR_R1, PIN_MOTOR_R2, PIN_MOTOR_R3};
int* pin_side [3] = {pins_left, pins_right};
int pins_7_digit [8] = {PIN_LED_A, PIN_LED_B, PIN_LED_C, PIN_LED_D, PIN_LED_E, PIN_LED_F, PIN_LED_G, PIN_LED_DP};

// This define the A/B/C/D/E/F/G/DP standard LEDS on/off of a 7 digit display
bool off[] = {0,0,0,0,0,0,0,0};
bool zero[] = {0,0,0,0,0,0,1,0};
bool one[] = {0,1,1,0,0,0,0,0};
bool two[] = {1,1,0,1,1,0,1,0};
bool three[] = {1,1,1,1,0,0,1,0};
bool four[] = {0,1,1,0,0,1,1,0};
bool five[] = {1,0,1,1,0,1,1,0};
bool six[] = {0,0,1,1,1,1,1,0};
bool seven[] = {1,1,1,0,0,0,0,0};
bool eight[] = {1,1,1,1,1,1,1,0};
bool nine[] = {1,1,1,1,0,1,1,0};
bool* display_7_digit[] = {zero, one, two, three, four, five, six, seven, eight, nine, off};


void setup() {
  Serial.begin(9600);
  // COIN MOTORS OUTPUTS
  pinMode(PIN_MOTOR_L1, OUTPUT);
  pinMode(PIN_MOTOR_L2, OUTPUT);
  pinMode(PIN_MOTOR_L3, OUTPUT);
  pinMode(PIN_MOTOR_R1, OUTPUT);
  pinMode(PIN_MOTOR_R2, OUTPUT);
  pinMode(PIN_MOTOR_R3, OUTPUT);
  // 7 DIGIT OUTPUT
  pinMode(PIN_LED_A, OUTPUT);
  pinMode(PIN_LED_B, OUTPUT);
  pinMode(PIN_LED_C, OUTPUT);
  pinMode(PIN_LED_D, OUTPUT);
  pinMode(PIN_LED_E, OUTPUT);
  pinMode(PIN_LED_F, OUTPUT);
  pinMode(PIN_LED_G, OUTPUT);
  pinMode(PIN_LED_DP, OUTPUT);
  //BUTTON OUTPUT
  pinMode(PIN_OUTPUT_BUTTON, OUTPUT);
  digitalWrite(PIN_OUTPUT_BUTTON, 1);
  
  //INPUTS
  pinMode(PIN_INPUT_BUTTON, INPUT_PULLUP); // button -> digital & event detection
  pinMode(PIN_INPUT_PIEZO_LEFT_PUSH, INPUT);
  pinMode(PIN_INPUT_PIEZO_LEFT_RELEASE, INPUT);
  pinMode(PIN_INPUT_PIEZO_RIGHT_PUSH, INPUT);
  pinMode(PIN_INPUT_PIEZO_RIGHT_RELEASE, INPUT);

  //Button event detection
  attachInterrupt(digitalPinToInterrupt(PIN_INPUT_BUTTON), change_mode, FALLING);
  
  digitalWrite(PIN_MOTOR_L1, 0);
  digitalWrite(PIN_MOTOR_L2, 0);
  digitalWrite(PIN_MOTOR_L3, 0);
  digitalWrite(PIN_MOTOR_R1, 0);
  digitalWrite(PIN_MOTOR_R2, 0);
  digitalWrite(PIN_MOTOR_R3, 0);

  digitalWrite(PIN_LED_A, 0);
  digitalWrite(PIN_LED_B, 0);
  digitalWrite(PIN_LED_C, 0);
  digitalWrite(PIN_LED_D, 0);
  digitalWrite(PIN_LED_E, 0);
  digitalWrite(PIN_LED_F, 0);
  digitalWrite(PIN_LED_G, 0);
  digitalWrite(PIN_LED_DP, 0);
  
}

void loop() {

  //run_it();
  //test_LED();
  //test_motors();
  //test_piezo();
  //test_button();
  motor_activation_proportionnal_to_push();
}
//------------------------------------->>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>    <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<---------------------------------------



void run_it(){
  side = !side;
  display_mode(activation_mode);
  if(activation_mode == 0){run_mode_0();}
  if(activation_mode == 1){run_mode_1();}
  if(activation_mode == 2){run_mode_2();}
  if(activation_mode == 3){run_mode_3();}
  if(activation_mode == 4){run_mode_4();}
  if(activation_mode == 5){run_mode_5();}
  if(activation_mode == 6){run_mode_6();}
}

void run_mode_0(){
  deactivate(pins_left);
  deactivate(pins_right);
}

void run_mode_1(){
  // Wake up stimulation every X seconds
  int time_between = 5 * 60 * 1000;
  snooze(time_between);
}

void run_mode_2(){

}

void run_mode_3(){

}

void run_mode_4(){

}

void run_mode_5(){

}

void run_mode_6(){
  // Press left Piezo DEactivate left actuators
  bool trigger = 0;

  float left = analogRead(PIN_INPUT_PIEZO_LEFT_PUSH);
  float right = analogRead(PIN_INPUT_PIEZO_RIGHT_PUSH);
  
  if(left > 150.0){
    trigger = 1;
    delay(1000);
    for(int i=0; i<3; i++){
      digitalWrite(pins_right[i], 1);
    }
    while(trigger){
      if(analogRead(PIN_INPUT_PIEZO_RIGHT_PUSH) > 150){
        trigger = 0;
        delay(300);
        for(int i=0; i<3; i++){
          digitalWrite(pins_right[i], 0);
        }
      }
    }
  }
  if(right > 150){
    trigger = 1;
    delay(1000);
    for(int i=0; i<3; i++){
      digitalWrite(pins_left[i], 1);
    }
    while(trigger){
      if(analogRead(PIN_INPUT_PIEZO_LEFT_PUSH) > 150){
        trigger = 0;
        delay(300);
        for(int i=0; i<3; i++){
          digitalWrite(pins_left[i], 0);
        }
      }
    }
  }
}

//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> MODES MANAGEMENT <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

void change_mode(){
  static unsigned long last_interrupt_time = 0;
  unsigned long interrupt_time = millis();
  if (interrupt_time - last_interrupt_time > 500){
    if(activation_mode < MAX_MODE){
        activation_mode++;
      }else{
        activation_mode = 0;
      }
  }
  last_interrupt_time = interrupt_time;
}

//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> 7 DIGIT <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

void display_slot(int num, int SLOT){
  clear_7_digit();
  digitalWrite(SLOT, 0);
  for(int j=0; j<8; j++){
    digitalWrite(pins_7_digit[j], display_7_digit[num][j]);
  }
}

void display_steps(int steps){
  int duration = 100;
  
  int slot1 = steps%10;
  int slot2 = (steps%100) / 10;
  int slot3 = (steps%1000) / 100;
  int slot4 = (steps%10000) / 1000;

  display_slot(slot1, SLOT_1); delay(duration); 
  display_slot(slot2, SLOT_2); delay(duration);
  display_slot(slot3, SLOT_3); delay(duration);
  display_slot(slot4, SLOT_4); delay(duration);
}

void display_mode(int num){
  int duration = 300;
  for(int i=0; i<3; i++){
    display_slot(num, SLOT_1);
    delay(duration);
    digitalWrite(SLOT_1, 1);
    delay(duration);
  }
  clear_7_digit();
}

void clear_7_digit(){
  display_slot(10, SLOT_1);
  activate_all_slot();
}

void activate_all_slot(){
  digitalWrite(SLOT_1, 1);
  digitalWrite(SLOT_2, 1);
  digitalWrite(SLOT_3, 1);
  digitalWrite(SLOT_4, 1);
}

void deactivate_all_slot(){
  digitalWrite(SLOT_1, 0);
  digitalWrite(SLOT_2, 0);
  digitalWrite(SLOT_3, 0);
  digitalWrite(SLOT_4, 0);
}

//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> BASIC MOTOR ACTIVATION <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

void activate(int* pins, int pwm){
  for(int i=0; i<3; i++){
    analogWrite(pins[i], pwm);
  }
}

void deactivate(int* pins){
    for(int i=0; i<3; i++){
    analogWrite(pins[i], 0);
  }
}

void activate_progressive(){
    // L/R sequential <PWM> activation Back to Front
    int* pins = pin_side[side];
    int d = speed/3;
    for(int i=0; i<3; i++){
      for(int j=0; j<10; j++){
        analogWrite(pins[i], 85+17*j);
        delay(d/10);
      }
      analogWrite(pins[i], 0);
    }
}

void deactivate_progressive(){
    // L/R sequential <PWM> DESactivation Back to Front
    int* pins = pin_side[side];
    int d = speed/3;
    for(int k=0; k<3; k++){digitalWrite(pins[k], 1);}
    for(int i=0; i<3; i++){
      for(int j=0; j<10; j++){
        analogWrite(pins[i], (255-17*j));
        delay(d/10);
      }
      analogWrite(pins[i], 0);
    }
}

void snooze(int time_between){
  static unsigned long last_run = 0;
  unsigned long current_run = millis();
  if (current_run - last_run > time_between){
    activate(pins_left, 255);
    activate(pins_right, 255);
    delay(500);
    deactivate(pins_left);
    deactivate(pins_right);
    last_run = current_run;
  }

}

//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> PIEZO FEEDBACK LOOP <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

void motor_activation_proportionnal_to_push(){
  float left = analogRead(PIN_INPUT_PIEZO_LEFT_PUSH);
  float right = analogRead(PIN_INPUT_PIEZO_RIGHT_PUSH);

  Serial.print("LEFT : ");Serial.print(left);Serial.print("\tRIGHT : ");Serial.println(right);
  
  if(left>70){
    int pwm_left = 81 + left/2;
    if(pwm_left>255){pwm_left=255;}
    activate(pins_left, pwm_left);
  }else{
    deactivate(pins_left);
  }
  if(right>70){
    int pwm_right = 81 + right/2;
    if(pwm_right>255){pwm_right=255;}
    activate(pins_right, pwm_right);
  }else{
    deactivate(pins_right);
  }
}

void step_count(){
  int treshold = 100;
  bool left = 0;
  bool right = 0;

  float left_trigger = analogRead(PIN_INPUT_PIEZO_LEFT_PUSH);
  float right_trigger = analogRead(PIN_INPUT_PIEZO_RIGHT_PUSH);

  if(left_trigger>treshold && left==0){
    activate(pins_left, 255);
    delay(200);
    deactivate(pins_left);
    left=1;
    steps++;
  }else{
    deactivate(pins_left);
  }
  if(right_trigger>treshold && right==0){
    activate(pins_right, 255);
    delay(200);
    deactivate(pins_right);
    right=1;
    steps++;
  }else{
    deactivate(pins_right);
  }
}

//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> UNIT TESTS <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

void test_LED(){
  while(1){
    delay(5000);
    digitalWrite(PIN_LED_A, 1);
    delay(1000);
    digitalWrite(PIN_LED_B, 1);
    delay(1000);
    digitalWrite(PIN_LED_C, 1);
    delay(1000);
    digitalWrite(PIN_LED_D, 1);
    delay(1000);
    digitalWrite(PIN_LED_E, 1);
    delay(1000);
    digitalWrite(PIN_LED_F, 1);
    delay(1000);
    digitalWrite(PIN_LED_G, 1);
    delay(1000);
    digitalWrite(PIN_LED_DP, 1);
    delay(1000);
    
    digitalWrite(PIN_LED_A, 0);
    digitalWrite(PIN_LED_B, 0);
    digitalWrite(PIN_LED_C, 0);
    digitalWrite(PIN_LED_D, 0);
    digitalWrite(PIN_LED_E, 0);
    digitalWrite(PIN_LED_F, 0);
    digitalWrite(PIN_LED_G, 0);
    digitalWrite(PIN_LED_DP, 0);
  }
}

void test_motors(){
  while(1){
    for(int i=0; i<3; i++){
      digitalWrite(pins_left[i], 1);
      delay(5000);
      digitalWrite(pins_left[i], 0);
    }
      for(int i=0; i<3; i++){
      digitalWrite(pins_right[i], 1);
      delay(5000);
      digitalWrite(pins_right[i], 0);
    }
  }
}

void test_piezo(){
  Serial.print("Left_push :");Serial.print(analogRead(PIN_INPUT_PIEZO_LEFT_PUSH));Serial.print("\tLeft_Release : ");Serial.print(analogRead(PIN_INPUT_PIEZO_LEFT_RELEASE));
  Serial.print("\t\tRight_push : ");Serial.print(analogRead(PIN_INPUT_PIEZO_RIGHT_PUSH));Serial.print("\tRight_Release : ");Serial.println(analogRead(PIN_INPUT_PIEZO_RIGHT_RELEASE));
}

void test_button(){
  Serial.print("  Mode : ");Serial.println(activation_mode);
}
