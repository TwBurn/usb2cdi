/*
 * Sketch for the CD-i Controller Library using a MagicNS adapter with a Switch Pro/Switch SNES controller - 2 Players
 * By Jeffrey Janssen - cdi@nmotion.nl
 */
 
#include <usbhub.h>
#include <CdiController.h>
#include <SPI.h>
#include "NS2CDi.h"

USB Usb;
USBHub UsbHub(&Usb);

NS2CDi cdiPlayer0(&Usb, 2, 3, 0); //Controller 1 -> RTS = 2, RXD = 3 
NS2CDi cdiPlayer1(&Usb, 6, 7, 1); //Controller 2 -> RTS = 6, RXD = 7

void setup() {
	//Set pin 4 and 5 to support the level converter
	pinMode(4, OUTPUT);
	pinMode(5, OUTPUT);
	digitalWrite(4, HIGH); //LV
	digitalWrite(5, LOW);  //GND

	Usb.Init();
	
	cdiPlayer0.Init();
	cdiPlayer1.Init();
}

void loop() { 
	Usb.Task();
	cdiPlayer0.Task();
	cdiPlayer1.Task();
}
