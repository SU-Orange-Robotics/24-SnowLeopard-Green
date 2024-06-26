/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       zhuowz                                                    */
/*    Created:      10/6/2023, 6:48:52 PM                                     */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#include "vex.h"
#include "drive.h"
#include "robot-config.h"
#include "intakeCat.h"
// #include "wings.h"
#include "autonomous.h"
#include "odometry.h"
#include "../seed/include/orange-odometry.h"

using namespace vex;

/*---------------------------------------------------------------------------*/
/*                          Pre-Autonomous Functions                         */
/*                                                                           */
/*  You may want to perform some actions before the competition starts.      */
/*  Do them in the following function.  You must return from this function   */
/*  or the autonomous and usercontrol tasks will not be started.  This       */
/*  function is only called once after the V5 has been powered on and        */
/*  not every time that the robot is disabled.                               */
/*---------------------------------------------------------------------------*/

void pre_auton(void) {
  // All activities that occur before the competition starts
  // Example: clearing encoders, setting servo positions, ...

  LeftMotorA.setStopping(brakeType::brake);
  LeftMotorB.setStopping(brakeType::brake);
  RightMotorA.setStopping(brakeType::brake);
  RightMotorB.setStopping(brakeType::brake);
  
  double maxCurrent = 2.5; //hardware maximum current is 2.5A

  LeftMotorA.setMaxTorque(maxCurrent, currentUnits::amp);
  LeftMotorB.setMaxTorque(maxCurrent, currentUnits::amp);
  RightMotorA.setMaxTorque(maxCurrent, currentUnits::amp);
  RightMotorB.setMaxTorque(maxCurrent, currentUnits::amp);

  ballKicker.setStopping(brakeType::hold);

  catapultA.setStopping(brakeType::hold);
  catapultB.setStopping(brakeType::hold);

  intake.setStopping(brakeType::brake);

  climberA.setStopping(brakeType::brake);
  climberB.setStopping(brakeType::brake);

  climberArm.setStopping(brakeType::brake);
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              Autonomous Task                              */
/*                                                                           */
/*  This task is used to control your robot during the autonomous phase of   */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/
// === CHECK autonomous.h and autonomous.cpp for the ACTUAL autonomous code ===
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              User Control Task                            */
/*                                                                           */
/*  This task is used to control your robot during the user control phase of */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

// this should be running in parallel with the other user control tasks
void lockCat() {
  // ==== WORK IN PROGRESS ====
  // double catapultRotVal = catapultRot.angle(rotationUnits::deg);
  // catapultLower(30);
  // while (catapultRot.angle(rotationUnits::deg) > 75) {
  //   wait(10, msec);

  //   if (abs(catapultRot.angle(rotationUnits::deg) - catapultRotVal) < 1) {
  //     break;
  //   } else {
  //     catapultRotVal = catapultRot.angle(rotationUnits::deg);
  //   }
  // }
  // catapultStop();
}

bool toggleNewWings = true;
void usercontrol(void) {
  // User control code here, inside the loop

  // use this if we have calibration issues. make sure it prevents driver control for at least 2 seconds
  //IMU.calibrate(2000);

  // lower catapult to a safe position
  thread t1(lowerCat);

  // pre-while loop setup
  //intak
  Controller1.ButtonL1.pressed([](){
    intakeSpin(true);
  });

  Controller1.ButtonL1.released(intakeStop);

  Controller1.ButtonL2.pressed([]() {
    intakeSpin();
  });

  Controller1.ButtonL2.released(intakeStop);

  // catapult
  // Controller1.ButtonR1.pressed([](){
  //   // catapultLaunch();
  //   // // these three lines here are what does the automatic arming of the catapult.
  //   // wait(50, msec);
  //   // waitUntil(getCatAccel() <= 0.1); // <-- might be blocking, which isnt desirable
  //   // catapultArm();
  // });

  // Controller1.ButtonR1.released([](){
  //   lowerCat();
  //   // catapultStop();
  // });

  // Controller1.ButtonR2.pressed([](){
  //   lowerCat();
  //   // if (!catInPosArmed()) {
  //   //   catapultArm();
  //   // }

  //   // catapultLower();
  // });

  // Controller1.ButtonR2.released([](){

  // });
  
  Controller1.ButtonDown.pressed([](){
    catapultLower();
  });

  Controller1.ButtonDown.released([](){
    stopAutoArming();
    catapultStop();
  });

  Controller1.ButtonUp.pressed([](){
    catapultRaise();
  });

  Controller1.ButtonUp.released([](){
    stopAutoArming();
    catapultStop();
  });

  //the button formerly known as twitter
  // Controller1.ButtonX.pressed([](){
  //   drive.toggleInvertedDrive();
  // });

  // Controller1.ButtonB.pressed([](){
  //   soleA.set(false);
  // });

  // Controller1.ButtonY.pressed([](){
  //   soleA.set(toggleNewWings);
  //   toggleNewWings = !toggleNewWings;
  // });



  while (1) {
    // This is the main execution loop for the user control program.
    // Each time through the loop your program should update motor + servo
    // values based on feedback from the joysticks.

    // ........................................................................
    // Insert user code here. This is where you use the joystick values to
    // update your motors, etc.
    // ........................................................................

    //drive.tankDrive(Controller1.Axis3.position(), Controller1.Axis2.position());

    // 1 stick arcade
    //drive.arcadeDrive(Controller1.Axis3.position(), Controller1.Axis4.position());

    // 2 stick arcade
    drive.arcadeDrive(Controller1.Axis3.position(), Controller1.Axis1.position());


    // climber stuff temporarily
    double climbSpeed = 0;
    double climbArmSpeed = 0;
    if (Controller1.ButtonR1.pressing()) {
      climbSpeed = 100;
    } else if (Controller1.ButtonR2.pressing()) {
      climbSpeed = -100;
    } else {
      climbSpeed = 0;
    }

    if (Controller1.ButtonX.pressing()) {
      climbArmSpeed = 40;
    } else if (Controller1.ButtonB.pressing()) {
      climbArmSpeed = -40;
    } else {
      climbArmSpeed = 0;
    }

    climberA.spin(directionType::fwd, climbSpeed, percentUnits::pct);
    climberB.spin(directionType::rev, climbSpeed, percentUnits::pct);
    climberArm.spin(directionType::fwd, climbArmSpeed, percentUnits::pct);


    if (Controller1.ButtonRight.pressing()){
      ballKicker.spin(directionType::fwd, 100, percentUnits::pct);
    } else if (Controller1.ButtonLeft.pressing()){
      ballKicker.spin(directionType::rev, 100, percentUnits::pct);
    } else {
      ballKicker.stop();
    }

    wait(20, msec); // Sleep the task for a short amount of time to
                    // prevent wasted resources.
  }
}

//
// Main will set up the competition functions and callbacks.
//



int main() {
  pre_auton();
  // Run the pre-autonomous function.
  Controller1.Screen.clearScreen();
  
  // Competition.autonomous(autonomous_competition);
  Competition.autonomous(autonomous_head_to_head);
  Competition.drivercontrol(usercontrol);

  // Prevent main from exiting with an infinite loop.
  while (true) {
    updateCatAccel(0.02);
    // odomUpdate();

    // Controller1.Screen.clearScreen();
    Controller1.Screen.setCursor(1,1);
    Controller1.Screen.print("heading %f", imu.heading());
    // Controller1.Screen.print(catapultRot.angle());
    // Controller1.Screen.print(gpsHeadingRad());
    // Controller1.Screen.setCursor(1,10);
    // Controller1.Screen.print(Brain.Battery.capacity());
    // Controller1.Screen.setCursor(2,1);
    // Controller1.Screen.print(getX());
    // Controller1.Screen.setCursor(3,1);
    // Controller1.Screen.print(getY());
    // Controller1.Screen.setCursor(2,12);
    //Controller1.Screen.print(drive.getAngleToPoint(0, 1000));
    // Controller1.Screen.print(catapultRot.angle());

    this_thread::sleep_for(200);
  }
}
