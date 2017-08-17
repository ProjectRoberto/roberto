#pragma once
#include <Arduino.h>
#include <phys253.h>
#include <LiquidCrystal.h>
//
enum State {stage_select , start, gate, hill, top, circle, zipline};
enum Stage {left,right};


class Robot
{
    public:
    //Singleton Class
    static Robot & Instance(){ static Robot instance; return instance; }
    //
    bool Init();
    void Retrieve_Agent();
    void Raise_Bucket();
    void Stop();
    void Go();
    void Display(char* message);
    double Speed(){ if(robot_stop){ return 0.0; } else{ return robot_speed; }}
    void Set_Speed(double v){ robot_speed = v; }
    void Blink_Left();
    void Blink_Right();
    void Stop_Blink();
    //
    void Set_State(State new_state);
    State Get_State(){ return state; }
    //Tie Spent in a certin state
    unsigned long Get_State_Time(){ return millis() - state_timer;}
    protected:
    Robot(){};
    private:
    State state = stage_select;
    unsigned long state_timer = 0;
    double robot_speed = 0.11;
    bool robot_stop = false;
    //Pins
    const int left_red = 10;
    const int far_left_green = 9;
    const int left_green = 9;
    const int right_red = 13;
    const int far_right_green = 12;
    const int right_green = 11;
    //
    unsigned long blink_timer = millis();
    bool blink_toggle = false;
    unsigned long blink_period = 600;


};
