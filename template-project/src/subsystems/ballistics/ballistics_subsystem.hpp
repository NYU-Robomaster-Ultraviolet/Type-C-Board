#ifndef BALLISTICS_SUBSYSTEM_HPP_
#define BALLISTICS_SUBSYSTEM_HPP_

#include "tap/control/subsystem.hpp"
#include "modm/math/filter/pid.hpp"
#include "tap/motor/dji_motor.hpp"
#include "tap/util_macros.hpp"
#include "controls/standard/standard_constants.hpp"
#include "tap/util_macros.hpp"
#include "tap/algorithms/math_user_utils.hpp"
#include "drivers.hpp"
#include "tap/algorithms/smooth_pid.hpp"
#include "tap/architecture/clock.hpp"
#include "controls/standard/imu_interface.hpp"


using namespace tap::algorithms;

namespace ballistics{
class BallisticsSubsystem : public tap::control::Subsystem
{
public:
    BallisticsSubsystem(src::Drivers *drivers)
    : tap::control::Subsystem(drivers), 
      targetYaw(0.0f),
      targetPitch(0.0f)
      {}

    void initialize()
    {
      pastTime = tap::arch::clock::getTimeMilliseconds();
      yawMotor.initialize();
      yawMotor.setDesiredOutput(0);
      pitchMotor.initialize();
      pitchMotor.setDesiredOutput(0);
      uint16_t currentPitchEncoder = pitchMotor.getEncoderUnwrapped();
      uint16_t currentYawEncoder = yawMotor.getEncoderUnwrapped();
      startingPitch = wrappedEncoderValueToRadians(currentPitchEncoder);
      startingYaw = wrappedEncoderValueToRadians(currentYawEncoder);
    }
    //try to push it
    //add some thing here
    //initilaizes the gimbal motors and make them not get any power


    void refresh() override;//Srikar

    void handle_auto_aim();//Nirav
    void calc_pitch_yaw(vector<vector<float>> target_info, vector<vector<float>> our1_info,float velocity,float barrel_len);//Nirav
    void make_angles(); //Lalit

    inline float wrappedEncoderValueToRadians(int64_t encoderValue)



private:
    //current gimbal pitch and yaw
    float startingPitch;
    float startingYaw;

    bool isTarget;
    bool autoaim;

    float barrelLength;
    float launchVelocity;

    float targetYaw;
    float targetPitch;

    tap::motor::DjiMotor yawMotor;
    tap::motor::DjiMotor pitchMotor;

  //current angles in radians
    float currentYaw;
    float currentPitch;

    

}; //class GimbalSubsystem
}//namespace gimbal


#endif