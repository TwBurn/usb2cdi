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

#define NS_DZ_AVG 128
#define NS_DZ_MAX (NS_DZ_AVG + 16)
#define NS_DZ_MIN (NS_DZ_AVG - 16)

#ifndef __NS2CDi_h__
#define __NS2CDi_h__

#include "MagicNS.h"

class NS2CDi {
	const int dSpeed[4] = {4, 8, 16, 32};
	const int aSpeed[4] = {8, 16, 32, 64};

	MagicNS magicNS;
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
	NS2CDi(USB *usb, byte pin_rts, byte pin_rxd, byte player): magicNS(usb), cdi(pin_rts, pin_rxd, MANEUVER, player) {}
	
	void Init() {
		cdi.Init();
	}

	bool Task() {
		cdi.Task();
	
		//Alter playerSpeed
		if      (magicNS.inputData.ZR || magicNS.inputData.R) setSpeed(+1);
		else if (magicNS.inputData.ZL || magicNS.inputData.L) setSpeed(-1);
		else allowSpeed = true;  
		
		//Analog Stick
		byte lhx = magicNS.inputData.LeftHatX;
		byte lhy = magicNS.inputData.LeftHatY;
		
		int x = map(lhx, 0, 255, -aSpeed[speed], +aSpeed[speed]);
		int y = map(lhy, 0, 255, -aSpeed[speed], +aSpeed[speed]);
		
		if (lhx > NS_DZ_MIN && lhx < NS_DZ_MAX) x = 0;
		if (lhy > NS_DZ_MIN && lhy < NS_DZ_MAX) y = 0;
		
		//Directional input . Overrides stick
		if (magicNS.inputData.LEFT)  x = -dSpeed[speed];
		if (magicNS.inputData.RIGHT) x = +dSpeed[speed];
		if (magicNS.inputData.UP)    y = -dSpeed[speed];
		if (magicNS.inputData.DOWN)  y = +dSpeed[speed];
		
		//Buttons
		bool bt_1 = (magicNS.inputData.A + magicNS.inputData.Y) != 0;
		bool bt_2 = (magicNS.inputData.B + magicNS.inputData.X) != 0;
		
		return cdi.JoyInput((byte)x, (byte)y, bt_1, bt_2);
	}
};

#endif
