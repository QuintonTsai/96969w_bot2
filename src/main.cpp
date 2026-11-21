#include "main.h"
#include "autons.hpp"
#include "claw.hpp"
#include "lift.hpp"
#include "robot.hpp"

/**
 * Runs as soon as the program starts. All other competition modes are blocked by initialize; keep it
 * under a few seconds.
 */
void initialize() {
	pros::lcd::initialize();
	chassis.calibrate();
	lift_init();
	// zero mechanism sensors here too, once at program start, so auton and driver control share a reference

	// set up here rather than in competition_initialize so it also works without a field/competition switch
	auton_selector_init();

	// show the odometry pose on lines 1-3 of the brain screen (line 0 is the auton selector),
	// used to measure/tune auton targets; add mechanism sensors on lines 4+
	pros::Task pose_display([]() {
		while (true) {
			lemlib::Pose pose = chassis.getPose();
			// LemLib tracks in inches; shown in cm to match driveRelative
			pros::lcd::print(1, "X: %.1f cm", pose.x * 2.54);
			pros::lcd::print(2, "Y: %.1f cm", pose.y * 2.54);
			pros::lcd::print(3, "Theta: %.1f deg", pose.theta);
			pros::delay(50);
		}
	});
}

/**
 * Runs while the robot is disabled by the field or competition switch.
 */
void disabled() {}

/**
 * Runs after initialize() and before autonomous when connected to the field or competition switch.
 */
void competition_initialize() {}

void autonomous() {
	run_selected_auton();
}

void opcontrol() {
	// LemLib motions run in their own task, so one still running when auton is cut off would keep
	// driving the motors and fight the joysticks until it times out
	chassis.cancelAllMotions();
	left_mg.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
	right_mg.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);

	while (true) {
		// arcade: left stick forward/back, right stick turns
		int dir = master.get_analog(ANALOG_LEFT_Y);
		int turn = master.get_analog(ANALOG_RIGHT_X);
		// desaturateBias favors turning authority over throttle when motors would otherwise saturate,
		// so turning stays responsive even at high forward/backward speed
		chassis.arcade(dir, turn, false, 0.5);

		// mechanism controls go here; for anything bigger than a button-to-motor mapping, put it in its
		// own file (like src/lift.cpp + include/lift.hpp) and call an update function from this loop
		lift_update();
		claw_update();

		// R2 held spins the intake, released stops it
		if (master.get_digital(DIGITAL_R2)) {
			intake_motor.move(127);
		} else {
			intake_motor.brake();
		}

		pros::delay(20);
	}
}
