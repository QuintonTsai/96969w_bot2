#pragma once

// lift driver control, in src/lift.cpp

// call once in initialize(): sets the lift to hold so it doesn't sag under load when released, and zeroes it
// the lift must be all the way down when the program starts
void lift_init();

// set the lift's current position as the bottom (the down limit); only call it with the lift all the way down
void lift_zero();

// call every opcontrol loop: R1 drives the lift up, R2 down until the down limit, neither (or both) holds it
void lift_update();
