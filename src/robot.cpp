#include "robot.hpp"

// TODO new bot: every port, cartridge and measurement below was copied from the old bot; check each one

pros::Controller master(pros::E_CONTROLLER_MASTER);

// negative port = reversed motor
pros::MotorGroup left_mg({9, -10}, pros::v5::MotorGears::blue); // TODO ports
pros::MotorGroup right_mg({-7, 8}, pros::v5::MotorGears::blue); // TODO ports
pros::Imu imu(11);                                               // TODO port

// the two lift motors face each other, so one is reversed; if the lift fights itself, swap which is negative
pros::MotorGroup lift_mg({13, -14}, pros::v5::MotorGears::green); // TODO ports

// if R2 spins the intake the wrong way, make the port negative (-15)
pros::Motor intake_motor(15, pros::v5::MotorGears::green);

// claw pistons on the brain's 3-wire (ADI) ports
pros::adi::DigitalOut claw_grip('A');  // opens/closes the claw
pros::adi::DigitalOut claw_raise('B'); // raises/lowers the claw

lemlib::Drivetrain drivetrain(&left_mg, &right_mg,
                              11.75,                      // TODO track width, in inches
                              lemlib::Omniwheel::NEW_325, // TODO wheel size
                              450,                        // TODO wheel rpm after external gearing
                              2);                         // horizontalDrift, 2 for all omnis, 8 with traction wheels

// tracking wheels on rotation sensors; a negative port reverses the sensor
// if X/Y on the brain screen counts the wrong way when you push the robot, negate that port
pros::Rotation vertical_rotation(6);    // front (forward/back) tracking wheel
pros::Rotation horizontal_rotation(12); // sideways tracking wheel

// offset = distance from the tracking center (center of rotation) to the wheel, in inches
// vertical wheel: + if right of center, - if left; horizontal wheel: + if in front of center, - if behind
// 2.25" wheels (no LemLib preset for this size, so the diameter is written directly)
lemlib::TrackingWheel vertical_tracking_wheel(&vertical_rotation, 2.25, 3.85); // 3.85" right of center
lemlib::TrackingWheel horizontal_tracking_wheel(&horizontal_rotation, 2.25, 5); // 5" in front of center

lemlib::OdomSensors sensors(&vertical_tracking_wheel,   // vertical tracking wheel
                            nullptr,                    // no second vertical tracking wheel
                            &horizontal_tracking_wheel, // horizontal tracking wheel
                            nullptr,                    // no second horizontal tracking wheel
                            &imu);

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
