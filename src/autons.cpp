#include "autons.hpp"
#include "auton_helpers.hpp"
#include "robot.hpp"
#include <atomic>

// ---------------------------------------------------------------------------------------------------
// routines
// each one starts with auton_start(), then chains motions off target_heading (the heading it was
// aiming for) rather than the heading the last motion actually exited at
// ---------------------------------------------------------------------------------------------------

void right_autonomous() {
	auton_start();
	float target_heading = 0;

	// example, replace with the real route:
	// driveRelative(target_heading, 50, 1500);
	// chassis.waitUntilDone();
	// target_heading -= 90;
	// chassis.turnToHeading(target_heading, 1000);
	// chassis.waitUntilDone();
	(void)target_heading;
}

void left_autonomous() {
	auton_start();
	float target_heading = 0;
	(void)target_heading;
}

void no_autonomous() {}

// ---------------------------------------------------------------------------------------------------
// selector
// ---------------------------------------------------------------------------------------------------

struct Auton {
	const char* name;
	void (*run)();
};

// the first entry is selected by default
const Auton autons[] = {
	{"Right", right_autonomous},
	{"Left", left_autonomous},
	{"None", no_autonomous},
};
constexpr int auton_count = sizeof(autons) / sizeof(autons[0]);

std::atomic<int> selected_auton{0};

void render_auton_selector() {
	pros::lcd::print(0, "< Auton: %s >", autons[selected_auton.load()].name);
}

void select_previous_auton() {
	selected_auton.store((selected_auton.load() + auton_count - 1) % auton_count);
	render_auton_selector();
}

void select_next_auton() {
	selected_auton.store((selected_auton.load() + 1) % auton_count);
	render_auton_selector();
}

void auton_selector_init() {
	pros::lcd::register_btn0_cb(select_previous_auton);
	pros::lcd::register_btn2_cb(select_next_auton);
	render_auton_selector();
}

void run_selected_auton() {
	autons[selected_auton.load()].run();
}
