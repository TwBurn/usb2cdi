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


#ifndef __KeyMouse2CDi_h__
#define __KeyMouse2CDi_h__

#include <hidboot.h>

class CdiMouseParser : public MouseReportParser {
public:
	int dx     = 0;
	int dy     = 0;
	bool left  = false;
	bool right = false;

	void reset() { dx = 0; dy = 0; }
	
protected:
	void OnMouseEvent(MOUSEINFO *mi) {
		dx    += mi->dX;
		dy    += mi->dY;
		left   = mi->bmLeftButton | mi->bmMiddleButton;
		right  = mi->bmRightButton | mi->bmMiddleButton;
	}
	void OnMouseMove(MOUSEINFO *mi)        { OnMouseEvent(mi); };
	void OnLeftButtonUp(MOUSEINFO *mi)     { OnMouseEvent(mi); };
	void OnLeftButtonDown(MOUSEINFO *mi)   { OnMouseEvent(mi); };
	void OnRightButtonUp(MOUSEINFO *mi)    { OnMouseEvent(mi); };
	void OnRightButtonDown(MOUSEINFO *mi)  { OnMouseEvent(mi); };
};

class CdiKeyboardParser : public KeyboardReportParser {
public:
	int  speed   = 4;
	bool up      = false;
	bool down    = false;
	bool left    = false;
	bool right   = false;
	bool button1 = false;
	bool button2 = false;
	bool button3 = false;
protected:
	void OnKeyEvent (uint8_t key, bool pressed) {
		switch(key) {
			case 0x52: up      = pressed; break; //UP
			case 0x51: down    = pressed; break; //DOWN
			case 0x50: left    = pressed; break; //LEFT
			case 0x4F: right   = pressed; break; //RIGHT
			case 0x1D: button1 = pressed; break; //Z
			case 0x1B: button2 = pressed; break; //X
			case 0x06: button3 = pressed; break; //C
		}
	
		//Speed selection, use number keys "1" (0x1E) to "0" (0x27)
		if (pressed && key >= 0x1E && key <= 0x27) speed = key - 0x1E;
	}
	void OnKeyDown	(uint8_t mod, uint8_t key) { OnKeyEvent(key, true); };
	void OnKeyUp	(uint8_t mod, uint8_t key) { OnKeyEvent(key, false); };
};

class KeyMouse2CDi : public MouseReportParser, KeyboardReportParser {

private:
	HIDBoot<USB_HID_PROTOCOL_KEYBOARD | USB_HID_PROTOCOL_MOUSE> HidComposite;
	HIDBoot<USB_HID_PROTOCOL_KEYBOARD> HidKeyboard;
	HIDBoot<USB_HID_PROTOCOL_MOUSE>    HidMouse;

	CdiController cdi;
	CdiMouseParser mouse;
	CdiKeyboardParser keyboard;
	//Keyboard speeds:         1   2   3   4   5   6   7   8   9   0
	const int keySpeeds[10] = {1,  2,  4,  8, 12, 16, 20, 24, 28, 32};
public:
	KeyMouse2CDi(USB *usb, byte pin_rts, byte pin_rxd, byte player): HidComposite(usb), HidKeyboard(usb), HidMouse(usb), cdi(pin_rts, pin_rxd, RELATIVE, player) {}
	
	void Init() {
		cdi.Init();
		HidComposite.SetReportParser(0, &keyboard);
		HidComposite.SetReportParser(1, &mouse);
		HidKeyboard.SetReportParser(0, &keyboard);
		HidMouse.SetReportParser(0, &mouse);
	}

	bool Task() {
		cdi.Task();

		if (!cdi.IsConnected()) {
			mouse.reset();
		}
	
		int curSpeed = keySpeeds[keyboard.speed];
		int moveX = keyboard.right * curSpeed - keyboard.left * curSpeed + mouse.dx;
		int moveY = keyboard.down  * curSpeed - keyboard.up   * curSpeed + mouse.dy;

		bool button1 = keyboard.button1 | keyboard.button3 | mouse.left;
		bool button2 = keyboard.button2 | keyboard.button3 | mouse.right;

		moveX = constrain(moveX, -127, 127);
		moveY = constrain(moveY, -127, 127);
		
		if (cdi.JoyInput((byte)moveX, (byte)moveY, button1, button2)) {
			mouse.reset();
			return true;
		}
		else {
			return false;
		}
	}
};

#endif
