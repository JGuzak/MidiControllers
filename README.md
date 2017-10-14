# Midi Controllers
An open source library for midi controller developement

## The layout:

Usb-midi controllers with Arduinos have two main components; the controller state/logic and the usb-midi communication. Inside the Libraries folder is a collection of classes for handling some hardware inputs and midi output over the Arduino's serial port. These projects are using Hiduino for the usb-midi firmware.




### Libraries:

ControllerLib:
Made for the Arduino Due, this library uses hardware interrupts to trigger state changing logic.
