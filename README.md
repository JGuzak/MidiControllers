# Midi Controllers
An open source library for midi controller developement

### Controllers:

![breadboard version of the C1due](https://github.com/JGuzak/MidiControllers/blob/master/Controllers/C1Mono/proto%20C1%20for%20due/C1due%20(2).JPG)
C1due (C1Mono prototype)

![caseless version of the C1Mono](https://github.com/JGuzak/MidiControllers/blob/master/Controllers/C1Mono/proto%20C1%20for%20uno/protoboard_mount_1.jpg)
C1Mono without an enclosure.

More details [here](https://github.com/JGuzak/MidiControllers/tree/master/Controllers)


### Libraries:

* ControllerLib:
    Made for the Arduino Due, this library uses hardware interrupts to trigger state changing logic. Includes classes for handling rotary encoders and buttons.
* SparkFun SX1509:
    Made by SparkFun for handling I/O Expander over the IIRC bus. [Original source](https://github.com/sparkfun/SparkFun_SX1509_Arduino_Library)

More details [here](https://github.com/JGuzak/MidiControllers/tree/master/Libraries)


### Remote Maps:

Remote is a system designed by Propellerheads for integrating usb midi controllers into their DAW Reason.
Remote overview [here](https://www.propellerheads.se/support/user/reason/remote/)
Apply for free to become a Remote developer [here](https://www.propellerheads.se/developer/remote.php)

#### Custom Mappings: (Manufacturer - Model)

* DJTT - Midi Fighter 3D [files]()
* NI - Traktor D2 [files](https://github.com/JGuzak/MidiControllers/tree/master/RemoteMaps/Traktor%20D2) 
* Akai - MPK Mini [files](https://github.com/JGuzak/MidiControllers/tree/master/RemoteMaps/Akai%20ReMPC%20Mini%20Codec) 
* Arturia - MiniLab [files](https://github.com/JGuzak/MidiControllers/tree/master/RemoteMaps/Arturia%20MiniLab%20Codec) 
* AOTA - C1Mono [files](https://github.com/JGuzak/MidiControllers/tree/master/RemoteMaps/C1Mono)

More details [here](https://github.com/JGuzak/MidiControllers/tree/master/RemoteMaps)


### Arduino as ISP Shield:

![protoboard shield and ribbon cabble for ISP programming](https://github.com/JGuzak/MidiControllers/blob/master/uno_isp/uno_isp_shield%20(1).jpg)
A shield for using an Arduino as an ISP programmer to easily upload new firmware.

More details [here](https://github.com/JGuzak/MidiControllers/tree/master/uno_isp)
