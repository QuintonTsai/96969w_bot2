#pragma once

#include "main.h"
#include "lemlib/api.hpp"

// every motor, sensor and the LemLib chassis, defined in src/robot.cpp
// add new mechanisms' motors and sensors here (extern) and in robot.cpp (definition)

extern pros::Controller master;

extern pros::MotorGroup left_mg;
extern pros::MotorGroup right_mg;
extern pros::Imu imu;

extern lemlib::Chassis chassis;
