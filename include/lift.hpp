#pragma once

// lift driver control, in src/lift.cpp

// call once in initialize(): sets the lift to hold so it doesn't sag under load when released, and zeroes it
// the lift must be all the way down when the program starts
void lift_init();

// set the lift's current position as the bottom (the down limit); only call it with the lift all the way down
void lift_zero();

// call every opcontrol loop: L2 drives the lift up, L1 lets it fall by gravity (coast) until the down limit, neither (or both) holds it
void lift_update();
