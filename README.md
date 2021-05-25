# USB2CD-i
Compilation of Arduino Sketches for use with the CD-i Controller Library.

For use with the compact version described on https://github.com/TwBurn/cdi-controller-library/wiki.

## Usage

### Requirements
* CD-i Controller Library (https://github.com/TwBurn/cdi-controller-library)
* USB Host Shield Library (https://github.com/felis/USB_Host_Shield_2.0)

Because these sketches are designed to be transmitted to the Arduino without adjustments, there is quite a bit of duplicate/similar code.

## Contents

### KeyboardMouse-1p
Keyboard and/or mouse, mapped to a CD-i Relative Coordinate Device.

Mouse mapping:
* Mouse move: Move cursor
* Left button: Button 1.
* Right button: Button 2.
* Middle button: Button 1 and 2

Keyboard mapping:
* Cursor keys: Directional input
* Z: Button 1
* X: Button 2
* C: Button 1 and 2
* 0-9: Speed selection (Keyboard order, 1 is slowest, 0 is fastest)

### MagicNS-1p
Switch Pro, Switch SNES controller using a MagicNS dongle, mapped to a CD-i Maneuvering Device.

Button mapping:
* D-pad and left thumbstick: Directional input.
* A and X: Button 1.
* B and Y: Button 2.
* L/R triggers: Speed up/down (4 speed)

### MagicNS-2p
Switch Pro, Switch SNES controller using a MagicNS dongle, 2 player version.

Button mapping: See [MagicNS-1p](MagicNS-1p)

*Note: An USB hub and two dongles are needed.*

### WiiBT-1p
WiiMote/WiiU Pro controller using a Bluetooth dongle. A WiiMote must be held sideways.

Button mapping:
* D-pad and left thumbstick: Directional input.
* A, X and 1: Button 1.
* B, Y and 2: Button 2.
* +/-: Speed up/down (4 speed)

### X360USB-1p
XBox 360 controller connected via USB, mapped to a CD-i Maneuvering Device.

Button mapping:
* D-pad and left thumbstick: Directional input.
* A and X: Button 1.
* B and Y: Button 2.
* L/R triggers: Speed up/down (4 speed)

*Note: Untested*