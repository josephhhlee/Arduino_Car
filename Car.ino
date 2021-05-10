#include <SoftwareSerial.h>
SoftwareSerial HM10(2, 3); // RX = 2, TX = 3
int pwm_value;
char input;
char forward = 'W';
char reverse = 'S';
char right = 'D';
char left = 'A';
char forwardRight = 'E';
char forwardLeft = 'Q';
char breaking = 'F';
char dance = 'R';
#define motor1 5
#define fw1 8
#define bw1 9
#define motor2 6
#define fw2 10
#define bw2 11
#define one_complete_cycle 1000 // assuming 1 complete turn uses 1 second
#define one_quarter_cycle 250  // assuming 1/4 turn uses 0.25 second


void forward_car(char fw, char bw, char motor, int fast)
{
  digitalWrite(fw, LOW);
  digitalWrite(bw, HIGH);
  analogWrite(motor, fast);
}

void reverse_car(char fw, char bw, char motor, int fast)
{
  digitalWrite(fw, HIGH);
  digitalWrite(bw, LOW);
  analogWrite(motor, fast);
}

void right_car(int fast2, int fast1)
{
  forward_car(fw2,bw2,motor2,fast2);
  reverse_car(fw1,bw1,motor1,fast1);
}

void left_car(int fast1, int fast2)
{
  forward_car(fw1,bw1,motor1,fast1);
  reverse_car(fw2,bw2,motor2,fast2);
}

void stop_car()
{
  digitalWrite(fw1, LOW);
  digitalWrite(bw1, LOW);
  digitalWrite(fw2, LOW);
  digitalWrite(bw2, LOW);
  analogWrite(motor1, 0);
  analogWrite(motor2, 0);
}

void dance_car()
{
  // assuming Robot Car is facing the front direction
  right_car(255,255);
  delay(one_complete_cycle);
  reverse_car(fw1,bw1,motor1,255);
  reverse_car(fw2,bw2,motor2,255);
  delay(one_complete_cycle);
  right_car(255,255);
  delay(one_complete_cycle);
  left_car(255,255);
  delay(one_quarter_cycle);
  forward_car(fw1,bw1,motor1,255);
  forward_car(fw2,bw2,motor2,255);
  delay(one_complete_cycle);
  right_car(255,255);
  delay(one_quarter_cycle); // return Robot Car to face the front direction
  left_car(255,255);
  delay(one_complete_cycle);
  forward_car(fw1,bw1,motor1,255);
  forward_car(fw2,bw2,motor2,255);
  delay(one_complete_cycle);
  left_car(255,255);
  delay(one_complete_cycle);
  right_car(255,255);
  delay(one_quarter_cycle);
  forward_car(fw1,bw1,motor1,255);
  forward_car(fw2,bw2,motor2,255);
  delay(one_complete_cycle);
  left_car(255,255);
  delay(one_quarter_cycle);
}

void setup()
{
  Serial.begin(9600);
  HM10.begin(9600);
  pinMode(motor1, OUTPUT);
  pinMode(fw1, OUTPUT);
  pinMode(bw1, OUTPUT);
  pinMode(motor2, OUTPUT);
  pinMode(fw2, OUTPUT);
  pinMode(bw2, OUTPUT);

}

void loop()
{
  /*while(Serial.available() > 0)
  {
    input= Serial.read();
  }*/
  HM10.listen();  // listen the HM10 port
  while(HM10.available() > 0)
  {
    input = HM10.read();
    Serial.println(input);
  }
    if(input == forward) 
    {
      forward_car(fw1,bw1,motor1,180);
      forward_car(fw2,bw2,motor2,255);
      Serial.println("forward");
    }
    else if(input == reverse)
    {
      for(pwm_value = 100; pwm_value <= 255; pwm_value += 1)
      { 
        HM10.listen();
        input = HM10.read();
        //input= Serial.read();
        reverse_car(fw1,bw1,motor1,pwm_value);
        reverse_car(fw2,bw2,motor2,pwm_value);
        Serial.println("reverse");
        delay(100);
        if(HM10.available() > 0)
        {
          if(input != reverse)
          {
            break;
          }
        }
      }
    }
    else if(input == right)
    {
      right_car(200,200);
      Serial.println("sharp right");
    }
    else if(input == left)
    {
      left_car(200,200);
      Serial.println("sharp left");
    }
    else if(input == forwardRight)
    {
      forward_car(fw1,bw1,motor1,70);
      forward_car(fw2,bw2,motor2,255);
      Serial.println("fat right");
    }
    else if(input == forwardLeft)
    {
      forward_car(fw1,bw1,motor1,180);
      forward_car(fw2,bw2,motor2,100);
      Serial.println("fat left");
    }
    else if(input == breaking)
    {
      stop_car();
      Serial.println("fat break");
    }
    else if(input == dance)
    {
      dance_car();
      Serial.println("dancing");
    }
}
