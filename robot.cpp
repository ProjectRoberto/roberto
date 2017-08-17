#include "robot.h"

//Initiliaz robot
bool Robot::Init()
{
    RCServo1.write(170);
    RCServo0.write(70);

   //
  pinMode(left_red, OUTPUT);
  pinMode(far_left_green, OUTPUT);
  pinMode(far_left_green, OUTPUT);
  pinMode(right_red, OUTPUT);
  pinMode(far_right_green, OUTPUT);
  pinMode(right_green, OUTPUT);
}

//Collect and Agent
void Robot::Retrieve_Agent()
{
    //Parameters for claw
    const int delay_time = 400;
    const int claw_open = 90;
    const int claw_close = 160;
    const int arm_lowered = 58;
    const int arm_raised = 150; 
    //Open Claw
    RCServo0.write(claw_open);
    delay(delay_time);
    // Lower Arm
    RCServo1.write(arm_lowered);
    delay(delay_time);
    //Close Claw
    RCServo0.write(claw_close);
    delay(delay_time);
    //Raise Arm
    RCServo1.write(arm_raised);
    delay(delay_time);
    //Open Claw
    RCServo0.write(claw_open);
    delay(delay_time);
}
//Raise and lower rack and pinion
void Robot::Raise_Bucket()
{
    //Rack and Pinion Parameters
    const int raise_time = 3000;
    const int raise_speed = 250;
    //Raise Bucket
    motor.speed(2, raise_speed);
    motor.speed(3, raise_speed);
    delay(raise_time);
    //Hold at height
    motor.speed(2, 0);
    motor.speed(3, 0);
    delay(raise_time);
    //Lower Bucket
    motor.speed(2, -raise_speed);
    motor.speed(3, -raise_speed);
    delay(raise_time);
    //Turn off motors
    motor.speed(2, 0);
    motor.speed(3, 0);

}

void Robot::Display(char* message)
{
    LCD.clear(); LCD.home();
    LCD.setCursor(0, 0); LCD.print(message);
}

void Robot::Stop()
{
  motor.speed(0, 0 );
  motor.speed(1, 0);
  digitalWrite(left_red,HIGH);
  digitalWrite(right_red,HIGH);
  robot_stop = true;
  this->Stop_Blink();
}

void Robot::Go()
{
  digitalWrite(left_red,LOW);
  digitalWrite(right_red,LOW);
  robot_stop = false;
}

void Robot::Set_State(State new_state)
{
    if(new_state == start)          Display("Start");
    else if (new_state == stage_select)     Display("Track");
    else if (new_state == gate)     Display("Gate");
    else if (new_state == hill )    Display("Hill");
    else if (new_state == top)      Display("Top");
    else if (new_state == circle)   Display("Circle");
    else if (new_state == zipline)  Display("Zipline");

    state = new_state;
    state_timer = millis();
}

void Robot::Blink_Left()
{
  if(millis() - blink_timer > blink_period)
  {
    if(blink_toggle)
    {
      digitalWrite(far_left_green,LOW);
      digitalWrite(left_green,HIGH);
    }
    else
    {
      digitalWrite(far_left_green,HIGH);
      digitalWrite(left_green,LOW);
    }
    blink_timer = millis();
    blink_toggle = !blink_toggle;  
  }  
}

void Robot::Blink_Right()
{
  if(millis() - blink_timer > blink_period)
  {
    if(blink_toggle)
    {
      digitalWrite(far_right_green,LOW);
      digitalWrite(right_green,HIGH);
    }
    else
    {
      digitalWrite(far_right_green,HIGH);
      digitalWrite(right_green,LOW);
    }
    blink_timer = millis();
    blink_toggle = !blink_toggle;  
  }  
}

void Robot::Stop_Blink()
{
  digitalWrite(far_right_green,LOW);
  digitalWrite(right_green,LOW);
  
  digitalWrite(far_left_green,LOW);
  digitalWrite(left_green,LOW);
}
