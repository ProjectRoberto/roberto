#include <phys253.h>

//
#include "robot.h"
#include "tape_reader.h"


unsigned long pastTime;
unsigned long intervalTime = millis();
//State Times

unsigned long start_time = 3000;
unsigned long gate_to_bucket = 13000;
unsigned long bucket_to_zipline = 8500;
//
int filter_pin = 0;
int filter_threshold = 100;
int amplitude_pin = 5;
//
int agent_count = 0;

float followerSpeed = 0.00;
float G = 3.80;
float D = -5;
int 
unsigned long countdown = 0;
unsigned long countdown_time = 1150;
bool agent_found = false;
bool in_circle = false;
bool top_of_hil = false;

int turn_speed = 120;
Stage stage = left;
Robot& Roberto = Robot::Instance();
bool gate_check = false;
void setup() {
  #include <phys253setup.txt>
  //Set for serial printing
  Serial.begin(9600);
  //Set up pins
  pinMode(filter_pin, INPUT);
  pinMode(amplitude_pin, INPUT);
  //
  Roberto.Init();
  Roberto.Set_State(stage_select);
  Roberto.Stop();
}

void loop() 
{

  //
  unsigned long T = micros() - pastTime;
  pastTime = micros();
  //Update QRD readings
  Tape_Reader::Instance().Update_Readings();
  if (Roberto.Get_State() == stage_select)
  {
    if(startbutton())
    {
      if(knob(6) > 500)
      {
        stage = left;
        start_time = 1500;
        gate_to_bucket += 1500;
        filter_threshold = 100;
        Roberto.Display("Left");  
      }
      else
      {
        stage = right;
        Tape_Reader::Instance().threshold = 700;
        filter_threshold = 100;
        
        Roberto.Display("Right");    
      }
      delay(500);
      Roberto.Go();
      Roberto.Set_State(start);
    }    
  }

  
  //Go forward for a certin amount of time
  if (Roberto.Get_State() == start)
  {

    if (Roberto.Get_State_Time() > start_time)
    {
      Roberto.Set_State(gate);
      Roberto.Stop();
      delay(500);

    }
  }
  if (Roberto.Get_State() == gate)
  {
    int amplitude_reading = analogRead(amplitude_pin);
    int filter_reading = analogRead(filter_pin);
    LCD.clear(); LCD.home();
    LCD.setCursor(0, 0); LCD.print(filter_reading);
    delay(50);
    
    if (filter_reading > filter_threshold)
    {
      gate_check = true;
    }
    if(gate_check && filter_reading < filter_threshold)
    {
      Roberto.Go();
      Roberto.Set_State(hill);
      Roberto.Set_Speed(0.12);
    }

  }
  
  
  if (Roberto.Get_State() == hill)
  {
    if (Roberto.Get_State_Time() > gate_to_bucket)
    {
      Roberto.Set_State(circle);
      G = 3.5;
      Roberto.Set_Speed(0.09);
 
    }
  }
  
  if (Roberto.Get_State() == top || Roberto.Get_State() == circle)
  {
    Roberto.Blink_Right();
    
    if (Tape_Reader::Instance().Agent_Marker() && !agent_found)
    {
      if (!in_circle)
      {
        delay(350);
        if(stage  == right)
        {
          motor.speed(0, -turn_speed );
          motor.speed(1, +turn_speed);       
        }
        if(stage == left)
        {
          motor.speed(0, +turn_speed );
          motor.speed(1, -turn_speed);            
        }
       
        Tape_Reader::Instance().Update_Readings();
        while (!Tape_Reader::Instance().On_Tape())
        {
          Tape_Reader::Instance().Update_Readings();
        }
        in_circle = true;
        Roberto.Display("In Circle");
      }
      else
      {

          Roberto.Display("Found Agent");
          agent_found = true;
          countdown = millis();
        
      }
    }
    if (millis() - countdown > countdown_time && agent_found)
    {
      if((agent_count == 7 && stage == left) || (agent_count == 6 && stage == right))
      {
        Roberto.Set_State(zipline);
        Roberto.Stop_Blink();
        
      }
      else
      {
        agent_found = false;
        agent_count++;
        
        Roberto.Display("Agent");
        if(agent_count != 1 || stage == right)
        { 
          Roberto.Stop();
          Roberto.Retrieve_Agent();
          Roberto.Go();
        }    
        
      }
    }
  }
  if(Roberto.Get_State() == zipline)
  {
    while(true)
    { 
      Tape_Reader::Instance().Update_Readings();
      if (Roberto.Get_State_Time() < bucket_to_zipline)
      {
        if(stage == left)
        {
          motor.speed(1, 80);
          motor.speed(0, 90);
        }
        else if (stage == right)
        {
          motor.speed(1, 88);
          motor.speed(0, 80);       
        }
        if(!Tape_Reader::Instance().On_Surface())
        {
          Roberto.Stop();
          while(true)
          {
            delay(100);
            Roberto.Display("Off Surface");
          }
        }
      }
      else 
      {
        motor.speed(1, -80);
        motor.speed(0, -80);
        delay(2000);
        motor.speed(1, 0);
        motor.speed(0, 0);
        Roberto.Set_Speed(0.0);
        while(true)
        {
          Roberto.Stop();
          delay(100);
           
        }
        Roberto.Go();
      }
    }
  }
  //Error from QRDS
  float error = Tape_Reader::Instance().Error();
  float pastError = Tape_Reader::Instance().Past_Error();
  //PID control
  float p = G * error;
  float d = (error - pastError) * (D / T);

  double dO =   p  +  d;
  int leftSpeed = Roberto.Speed() * (1.0 - dO / 10) * 1023 ;
  int rightSpeed = Roberto.Speed() * (1.0 + dO / 10) * 1023;

  motor.speed(0, leftSpeed );
  motor.speed(1, rightSpeed);


}
