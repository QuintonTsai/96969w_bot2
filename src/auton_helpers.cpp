#include "auton_helpers.hpp"
#include "robot.hpp"
#include <cmath>

void auton_start() {
	chassis.setPose(0, 0, 0);
	left_mg.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
	right_mg.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
}

// LemLib heading is clockwise from +Y, so forward is (sin, cos) and right is (cos, -sin)
lemlib::Pose relativePose(float heading_deg, float forward_cm, float right_cm, float turn_deg) {
	// LemLib works in inches
	float forward_in = forward_cm / 2.54f;
	float right_in = right_cm / 2.54f;
	lemlib::Pose pose = chassis.getPose();
	float heading_rad = lemlib::degToRad(heading_deg);
	return lemlib::Pose(pose.x + forward_in * std::sin(heading_rad) + right_in * std::cos(heading_rad),
	                    pose.y + forward_in * std::cos(heading_rad) - right_in * std::sin(heading_rad),
	                    heading_deg + turn_deg);
}

void driveRelative(float heading_deg, float distance_cm, int timeout, float max_speed, float min_speed) {
	lemlib::Pose target = relativePose(heading_deg, distance_cm, 0, 0);
	chassis.moveToPose(target.x, target.y, heading_deg, timeout,
	                   {.forwards = distance_cm >= 0, .lead = 0, .maxSpeed = max_speed, .minSpeed = min_speed});
}
