#include "robot.hpp"

// TODO new bot: every port, cartridge and measurement below was copied from the old bot; check each one

pros::Controller master(pros::E_CONTROLLER_MASTER);

// negative port = reversed motor
pros::MotorGroup left_mg({-9, -10}, pros::v5::MotorGears::blue); // TODO ports
pros::MotorGroup right_mg({1, 2}, pros::v5::MotorGears::blue);   // TODO ports
pros::Imu imu(19);                                               // TODO port

lemlib::Drivetrain drivetrain(&left_mg, &right_mg,
                              11.75,                      // TODO track width, in inches
                              lemlib::Omniwheel::NEW_325, // TODO wheel size
                              450,                        // TODO wheel rpm after external gearing
                              2);                         // horizontalDrift, 2 for all omnis, 8 with traction wheels

// no tracking wheels; odometry falls back to drive motor encoders + IMU
lemlib::OdomSensors sensors(nullptr, nullptr, nullptr, nullptr, &imu);

// TODO retune: these were tuned on the old bot; they're a starting point, not right for a new one
lemlib::ControllerSettings lateral_controller(8.2, // proportional gain (kP)
                                              0,   // integral gain (kI)
                                              35,  // derivative gain (kD)
                                              3,   // integral anti windup range
                                              1,   // small error range, in inches
                                              100, // small error range timeout, in ms
                                              3,   // large error range, in inches
                                              500, // large error range timeout, in ms
                                              20); // maximum acceleration (slew)

lemlib::ControllerSettings angular_controller(2.12,  // proportional gain (kP)
                                              0.082, // integral gain (kI)
                                              12,    // derivative gain (kD)
                                              3,     // integral anti windup range
                                              1,     // small error range, in degrees
                                              100,   // small error range timeout, in ms
                                              3,     // large error range, in degrees
                                              500,   // large error range timeout, in ms
                                              0);    // maximum acceleration (slew)

// input curve for throttle input during driver control
lemlib::ExpoDriveCurve throttle_curve(3,      // joystick deadband out of 127
                                      10,     // minimum output where drivetrain will move out of 127
                                      1.019); // expo curve gain

// input curve for steer input during driver control
lemlib::ExpoDriveCurve steer_curve(3,      // joystick deadband out of 127
                                   10,     // minimum output where drivetrain will move out of 127
                                   1.019); // expo curve gain

lemlib::Chassis chassis(drivetrain, lateral_controller, angular_controller, sensors, &throttle_curve, &steer_curve);
