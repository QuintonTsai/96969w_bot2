#pragma once

// claw driver control, in src/claw.cpp
// two pistons: ADI port A opens/closes the claw, ADI port B raises/lowers it

// call every opcontrol loop: tap R1 to open/close the claw, X raises it, A lowers it
void claw_update();
