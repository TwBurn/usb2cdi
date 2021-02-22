/*
 * Copyright (C) 2021 Jeffrey Janssen - cdi@nmotion.nl
 * 
 * This software may be distributed and modified under the terms of the GNU
 * General Public License version 2 (GPL2) as published by the Free Software
 * Foundation and appearing in the file GPL2.TXT included in the packaging of
 * this file. Please note that GPL2 Section 2[b] requires that all works based
 * on this software must also be made publicly available under the terms of
 * the GPL2 ("Copyleft").
 */

#define JOY_MIN 1000
#define JOY_MAX 3000

#define DEADZONE_AVG 2000
#define DEADZONE_RNG 200
#define DEADZONE_MAX (DEADZONE_AVG + DEADZONE_RNG)
#define DEADZONE_MIN (DEADZONE_AVG - DEADZONE_RNG)

#ifndef __WiiBT2CDi_h__
#define __WiiBT2CDi_h__

#include <Wii.h>

class WiiBT2CDi {
	const int dSpeed[4] = {4, 8, 16, 32};
	const int aSpeed[4] = {8, 16, 32, 64};

	WII Wii;
	CdiController cdi;
	int speed = 0;

	void setSpeed(int delta) {
		speed = min(3, max(0, speed + delta));
	}

public:
	WiiBT2CDi(BTD *btd, bool pair, byte pin_rts, byte pin_rxd, byte player): Wii(btd, pair), cdi(pin_rts, pin_rxd, MANEUVER, player) {}
	
	void Init() {
		cdi.Init();
	}

	bool Task() {
		cdi.Task();

		//Disconnect on HOME
		if (Wii.getButtonClick(HOME)) {
			Wii.disconnect();
		}
		
		//Alter playerSpeed
		if (Wii.getButtonClick(PLUS))  setSpeed(+1);
		if (Wii.getButtonClick(MINUS)) setSpeed(-1);
		
		//Analog Stick
		int lhx = Wii.getAnalogHat(LeftHatX);
		int lhy = Wii.getAnalogHat(LeftHatY);

		lhx = constrain(lhx, JOY_MIN, JOY_MAX);
		lhy = constrain(lhy, JOY_MIN, JOY_MAX);
		
		int x = map(lhx, JOY_MIN, JOY_MAX, -aSpeed[speed], +aSpeed[speed]);
		int y = map(lhy, JOY_MIN, JOY_MAX, -aSpeed[speed], +aSpeed[speed]);
		
		if (lhx > DEADZONE_MIN && lhx < DEADZONE_MAX) x = 0;
		if (lhy > DEADZONE_MIN && lhy < DEADZONE_MAX) y = 0;
		
		//Directional input -> Overrides stick
		if (Wii.wiimoteConnected) {
			//Assume WiiMote is held sideways
			if (Wii.getButtonPress(UP))   x = -dSpeed[speed];
			if (Wii.getButtonPress(DOWN)) x = +dSpeed[speed];
			if (Wii.getButtonPress(RIGHT))y = -dSpeed[speed];
			if (Wii.getButtonPress(LEFT)) y = +dSpeed[speed];
		}
		else {
			if (Wii.getButtonPress(LEFT))  x = -dSpeed[speed];
			if (Wii.getButtonPress(RIGHT)) x = +dSpeed[speed];
			if (Wii.getButtonPress(UP))    y = -dSpeed[speed];
			if (Wii.getButtonPress(DOWN))  y = +dSpeed[speed];
		}
		
		//Buttons
		bool bt_1 = Wii.getButtonPress(A) || Wii.getButtonPress(Y) || Wii.getButtonPress(ONE);
		bool bt_2 = Wii.getButtonPress(B) || Wii.getButtonPress(X) || Wii.getButtonPress(TWO);
		
		return cdi.JoyInput((byte)x, (byte)y, bt_1, bt_2);
	}
};

#endif
