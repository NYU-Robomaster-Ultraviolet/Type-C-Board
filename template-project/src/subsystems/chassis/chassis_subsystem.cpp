#include "chassis_subsystem.hpp"

#include "tap/communication/serial/remote.hpp"
#include "tap/algorithms/math_user_utils.hpp"

using namespace tap;

namespace chassis
{

void ChassisSubsystem::initialize()
{
    frontLeftMotor.initialize();
    frontRightMotor.initialize();
    backLeftMotor.initialize();
    backRightMotor.initialize();
}
void ChassisSubsystem::refresh() {
    updateRpmPid(&frontLeftPid, &frontLeftMotor, frontLeftDesiredRpm);
    updateRpmPid(&frontRightPid, &frontRightMotor, frontRightDesiredRpm);
    updateRpmPid(&backLeftPid, &backLeftMotor, backLeftDesiredRpm);
    updateRpmPid(&backRightPid, &backRightMotor, backRightDesiredRpm);
}
void ChassisSubsystem::updateRpmPid(modm::Pid<float>* pid, tap::motor::DjiMotor* const motor, float desiredRpm) {
    pid->update(desiredRpm - motor->getShaftRPM());
    motor->setDesiredOutput(pid->getValue());
}

/*
    Give desired setpoints for chassis movement. +x is forward, +y is right, +r is clockwise (turning right). Expressed in body frame.
*/
void ChassisSubsystem::setDesiredOutput(float x, float y, float r) 
{
    // x, y, and r contained between -1 and 1
    /*
    float norm = sqrt(x*x+y*y);
    if (norm > 1) {
        x = x / norm;
        y = y / norm;
    }
    */
    frontLeftDesiredRpm = tap::algorithms::limitVal<float>((x-y) * RPM_SCALE_FACTOR, -MAX_CURRENT_OUTPUT, MAX_CURRENT_OUTPUT);
    frontRightDesiredRpm = tap::algorithms::limitVal<float>((x+y) * RPM_SCALE_FACTOR, -MAX_CURRENT_OUTPUT, MAX_CURRENT_OUTPUT);
    backLeftDesiredRpm = tap::algorithms::limitVal<float>((x+y) * RPM_SCALE_FACTOR, -MAX_CURRENT_OUTPUT, MAX_CURRENT_OUTPUT);
    backRightDesiredRpm = tap::algorithms::limitVal<float>((x-y)* RPM_SCALE_FACTOR, -MAX_CURRENT_OUTPUT, MAX_CURRENT_OUTPUT);

    /*
    frontLeftDesiredRpm = tap::algorithms::limitVal<float>(
        ((x+y)+ (-CHASSIS_ROTATION_SET_SCALE - 1.0f) * CHASSIS_MOTOR_DISTANCE * r) * RPM_SCALE_FACTOR, -MAX_CURRENT_OUTPUT, MAX_CURRENT_OUTPUT);
    frontRightDesiredRpm = tap::algorithms::limitVal<float>(
        ((x-y) + (CHASSIS_ROTATION_SET_SCALE - 1.0f) * CHASSIS_MOTOR_DISTANCE * r) * RPM_SCALE_FACTOR, -MAX_CURRENT_OUTPUT, MAX_CURRENT_OUTPUT);
    backLeftDesiredRpm = tap::algorithms::limitVal<float>(
        ((-x+y) + (-CHASSIS_ROTATION_SET_SCALE - 1.0f) * CHASSIS_MOTOR_DISTANCE * r) * RPM_SCALE_FACTOR, -MAX_CURRENT_OUTPUT, MAX_CURRENT_OUTPUT);
    backRightDesiredRpm = tap::algorithms::limitVal<float>(
        ((-x-y) + (CHASSIS_ROTATION_SET_SCALE - 1.0f) * CHASSIS_MOTOR_DISTANCE * r) * RPM_SCALE_FACTOR, -MAX_CURRENT_OUTPUT, MAX_CURRENT_OUTPUT);
    */
}

} //namespace chassis