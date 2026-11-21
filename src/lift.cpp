#include "lift.hpp"
#include "robot.hpp"

// lowest the lift may be driven, in motor degrees from where it was zeroed (all the way down)
// raise it a little if the lift still slams into the bottom from momentum
constexpr double LIFT_DOWN_LIMIT = 0;

void lift_init() {
	lift_mg.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
	lift_mg.set_encoder_units_all(pros::E_MOTOR_ENCODER_DEGREES);
	lift_zero();
}

void lift_zero() {
	lift_mg.tare_position_all();
}

void lift_update() {
	bool up = master.get_digital(DIGITAL_L2);
	bool down = master.get_digital(DIGITAL_L1);
	// group position reads the first motor (port 13); the reversed one is already flipped so up is positive
	bool at_bottom = lift_mg.get_position() <= LIFT_DOWN_LIMIT;

	if (up && !down) {
		lift_mg.move(127);
	} else if (down && !up && !at_bottom) {
		// let the lift fall under its own weight instead of driving it down
		lift_mg.set_brake_mode_all(pros::E_MOTOR_BRAKE_COAST);
		lift_mg.brake();
	} else {
		// back to hold so the lift stays put when released (and at the bottom)
		lift_mg.set_brake_mode_all(pros::E_MOTOR_BRAKE_HOLD);
		lift_mg.brake();
	}
}
