#include <iostream>

const double Kp = 0.04; // ค่าคงที่ของ PID controller
const double Ki = 0.55;
const double Kd = 0.00;

double integal = 0.0;
double previousError = 0.0;
double setPointFlow;

double calculatePID(double setpointValue, double currentValue)
{
    double errorValue = setpointValue - currentValue;
    integal += errorValue;
    double derivativeValue = errorValue - previousError;
    previousError = errorValue;
    double PID_output = (Kp * errorValue) + (Kd * integal) + (Kd * derivativeValue);
    return PID_output;
}