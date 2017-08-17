#include "tape_reader.h"

Tape_Reader::Tape_Reader()
{
    //Set up the analog pins for reading
    pinMode(first_pin, INPUT);
    pinMode(first_pin + 1, INPUT);
    pinMode(first_pin + 2, INPUT);
    pinMode(first_pin + 3, INPUT);
}

void Tape_Reader::Update_Readings()
{
    //read the qrds
    right_agent_qrd = analogRead(first_pin);
    right_tape_qrd = analogRead(first_pin + 1);
    left_tape_qrd= analogRead(first_pin + 2);
    left_agent_qrd= analogRead(first_pin + 3);
}

int Tape_Reader::Error()
{
    int error = 0;
    //error
    if ((left_tape_qrd > threshold) && (right_tape_qrd > threshold)) error = 0;
    if ((left_tape_qrd > threshold) && (right_tape_qrd < threshold)) error = -1;
    if ((left_tape_qrd < threshold) && (right_tape_qrd > threshold)) error = +1;
    if ((left_tape_qrd < threshold) && (right_tape_qrd < threshold))
    {
        if ((left_agent_qrd > threshold) && ( right_agent_qrd < threshold))  error = -2;
        if ((left_agent_qrd < threshold) && ( right_agent_qrd > threshold))  error = 2; 
        else
        {
            if (past_error >= 0) error = 3;
            if (past_error <= 0) error = -3;
        }
    }
    past_error = error;
    //
    return error;
}

bool Tape_Reader::Agent_Marker()
{
    return (left_tape_qrd > threshold) && (right_tape_qrd > threshold) && (left_agent_qrd > threshold || right_agent_qrd > threshold);
}
bool Tape_Reader::On_Tape()
{
    return (left_tape_qrd > threshold) && (right_tape_qrd > threshold);
}
bool Tape_Reader::On_Surface()
{
  return (left_tape_qrd < threshold) || (right_tape_qrd < threshold) || (left_agent_qrd < threshold ) || (right_agent_qrd < threshold);  
}
void Tape_Reader::Print_QRDS()
{
    Serial.println("Left Agent QRD:");
    Serial.println(left_agent_qrd);
    Serial.println("Left QRD:");
    Serial.println(left_tape_qrd);
    Serial.println("Right QRD:");
    Serial.println(right_tape_qrd);
    Serial.println("Right Agent QRD:");
    Serial.println(right_agent_qrd);
}
