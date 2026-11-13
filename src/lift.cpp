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
	bool up = master.get_digital(DIGITAL_R1);
	bool down = master.get_digital(DIGITAL_R2);
	// group position reads the first motor (port 3); the reversed one is already flipped so up is positive
	bool at_bottom = lift_mg.get_position() <= LIFT_DOWN_LIMIT;

	if (up && !down) {
		lift_mg.move(100);
	} else if (down && !up && !at_bottom) {
		lift_mg.move(-100);
	} else {
		// brake() applies the hold brake mode set in lift_init
		lift_mg.brake();
	}
}
