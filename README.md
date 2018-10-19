# Arduino-H-Shifter-Pedals
Code for 3d printed H-Shifter

IMPORTANT!: This only works with a arduino Pro Micor or Leonardo since these are the only devices that support HID Mode!!!!!
Librarys needed:
Keypad
Arduino joystick
Pinout:
Buttons need to wired in a matrix pattern (saves you some pins on the arduino)


Button 1 to 4 need to be wired together on one side and the other contacts of the switches need to be connected to pin 8|7|6|5
The interconnected rail needs to be connected to pin 9

the other 4 buttons also just connect contact A together
The interconnected rail on the other side goes to Pin 1
The B contacts just need to be connected to the B contacts of their opposite buttons B contact (the ones that are individually connected to a seperate arduino pin)




The pedals are a "gift" by me since i use mapping for faders i got from an old audio mixer


You can edit the map from list to values you read from the analog read (divide the fader in 10 steps and just add the values to the map from array)
Maybe I'll add a detailed guide to this measurement in the future.
Just connect one side of the fader to the VCC pin and the other side to A0|A1|A2

-----------------------------------
Tip for using Pedals that are connected with the H shifter:
Get yourself a Double RJ-45 adaptor for extending LAN and cut the internal wires in half and use each end on the pedal and arduino side so you can unplug the pedals and use premade CAT cables


