#pragma once

#include "lemlib/api.hpp"

// call at the start of every auton routine: zeroes the pose and sets the drive to hold, so each motion
// stops where LemLib ends it instead of coasting into the next one
// also re-zero any mechanism sensors here; a zero taken in initialize() is off if the program was
// started with a mechanism raised
void auton_start();

// pose offset from the robot's current position, in the frame of heading_deg; distances in cm
// early-exited motions leave the robot a few degrees off, so callers pass the heading it was aiming for
lemlib::Pose relativePose(float heading_deg, float forward_cm, float right_cm, float turn_deg);

// drive straight along heading_deg; distances in cm, negative distance drives backward
// lower min_speed for a precise stop, keep it high to exit early at speed and flow into the next move
void driveRelative(float heading_deg, float distance_cm, int timeout, float max_speed = 127, float min_speed = 100);
