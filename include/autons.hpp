#pragma once

// auton routines and the brain-screen selector, in src/autons.cpp
// to add a routine: write the function in autons.cpp and add it to the `autons` list there

// show the selected auton on brain screen line 0 and hook up the left/right brain buttons to change it
void auton_selector_init();

// run whichever routine is selected
void run_selected_auton();
