#include "claw.hpp"
#include "robot.hpp"

// both pistons start retracted (false) when the program starts
// if a button does the opposite of what you expect, that's fine: each tap just flips the piston
static bool claw_grip_extended = false;

// which piston state lifts the claw; if X lowers it and A raises it, change this to false
constexpr bool CLAW_RAISED = true;

void claw_update() {
	// get_digital_new_press is true only on the loop the button goes down, so holding it doesn't keep toggling
	if (master.get_digital_new_press(DIGITAL_R1)) {
		claw_grip_extended = !claw_grip_extended;
		claw_grip.set_value(claw_grip_extended);
	}

	// X raises the claw, A lowers it; pressing the one it's already at does nothing
	if (master.get_digital_new_press(DIGITAL_X)) {
		claw_raise.set_value(CLAW_RAISED);
	} else if (master.get_digital_new_press(DIGITAL_A)) {
		claw_raise.set_value(!CLAW_RAISED);
	}
}
