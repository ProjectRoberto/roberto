#pragma once
#include <Arduino.h>

class Tape_Reader
{
    public: 
    //Singleton Class
    static Tape_Reader & Instance(){ static Tape_Reader instance; return instance; }
    //Updates QRD readings
    void Update_Readings();
    //Returns the error from the qrd readings
    int Error();
    //
    int Past_Error() { return past_error; }
    //Returns true on an agent marker
    bool Agent_Marker();
    //Returns true if the robot is on tape
    bool On_Tape();
    //
    bool On_Surface();
    //Prints to serial the QRD readings
    void Print_QRDS();
    
    Tape_Reader();
    
    const int first_pin = 1;
    int left_agent_qrd = 0;
    int left_tape_qrd = 0;
    int right_tape_qrd = 0;
    int right_agent_qrd = 0;
    //Past error
    int past_error = 0;
    int threshold = 600;
};
