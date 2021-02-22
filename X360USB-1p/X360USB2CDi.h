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

#define X360_DZ_AVG 0
#define X360_DS_RNG 7500
#define X360_DZ_MAX (X360_DZ_AVG + X360_DS_RNG)
#define X360_DZ_MIN (X360_DZ_AVG - X360_DS_RNG)

#ifndef __X360USB2CDi_h__
#define __X360USB2CDi_h__

#include <XBOXUSB.h>

class X360USB2CDi {
	const int dSpeed[4] = {4, 8, 16, 32};
	const int aSpeed[4] = {8, 16, 32, 64};

	XBOXUSB Xbox;
	CdiController cdi;
	int speed = 0;
	bool allowSpeed = true;

	void setSpeed(int delta) {
		if (allowSpeed) {
			speed = min(3, max(0, speed + delta));
			allowSpeed = false;
		}
	}

public:
	X360USB2CDi(USB *usb, byte pin_rts, byte pin_rxd, byte player): Xbox(usb), cdi(pin_rts, pin_rxd, MANEUVER, player) {}
	
	void Init() {
		cdi.Init();
	}

	bool Task() {
		cdi.Task();
	
		//Alter playerSpeed
		if      (Xbox.getButtonClick(R2)) setSpeed(+1);
		else if (Xbox.getButtonClick(L2)) setSpeed(-1);
		else allowSpeed = true;  
		
		//Analog Stick
		int lhx = Xbox.getAnalogHat(LeftHatX);
		int lhy = Xbox.getAnalogHat(LeftHatY);
		
		int x = map(lhx, -32767, 32767, -aSpeed[speed], +aSpeed[speed]);
		int y = map(lhy, -32767, 32767, -aSpeed[speed], +aSpeed[speed]);
		
		if (lhx > X360_DZ_MIN && lhx < X360_DZ_MAX) x = 0;
		if (lhy > X360_DZ_MIN && lhy < X360_DZ_MAX) y = 0;
		
		//Directional input . Overrides stick
		if (Xbox.getButtonPress(LEFT))  x = -dSpeed[speed];
		if (Xbox.getButtonPress(RIGHT)) x = +dSpeed[speed];
		if (Xbox.getButtonPress(UP))    y = -dSpeed[speed];
		if (Xbox.getButtonPress(DOWN))  y = +dSpeed[speed];
		
		//Buttons
		bool bt_1 = (Xbox.getButtonPress(B) + Xbox.getButtonPress(X)) != 0;
		bool bt_2 = (Xbox.getButtonPress(A) + Xbox.getButtonPress(Y)) != 0;
		
		return cdi.JoyInput((byte)x, (byte)y, bt_1, bt_2);
	}
};

#endif
