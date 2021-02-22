/*
 * Sketch for the CD-i Controller Library using a MagicNS adapter with a Switch Pro/Switch SNES controller - 2 Players
 * By Jeffrey Janssen - cdi@nmotion.nl
 */
 
#include <usbhub.h>
#include <CdiController.h>
#include "WiiBT2CDi.h"

USB Usb;
BTD Btd(&Usb);

WiiBT2CDi cdiPlayer(&Btd, PAIR, 2, 3, 0); //Controller 1 -> RTS = 2, RXD = 3

void setup() {
	//Set pin 4 and 5 to support the level converter
	pinMode(4, OUTPUT);
	pinMode(5, OUTPUT);
	digitalWrite(4, HIGH); //LV
	digitalWrite(5, LOW);  //GND

	Usb.Init();
	
	cdiPlayer.Init();
}

void loop() { 
	Usb.Task();
	cdiPlayer.Task();
}
