# C1Mono
![caseless version of the C1Mono](https://github.com/JGuzak/MidiControllers/blob/master/Controllers/C1Mono/proto%20C1%20for%20uno/protoboard_mount_1.jpg)

The C1Mono is a versitile usb-midi controller which features a set of 4 banked knobs and buttons which can control up to 16 knobs and 16 buttons in any host software that supports midi via usb. A special mapping has been made for Reason by Propellerheads which automatically maps the C1Mono controls to various device parameters in Reason. For instance, when a synthesizer is selected, the knobs and buttons will map to things like envelopes and filter controls. When device focus is switched to a reverb, the C1Mono updates its internal knob and button positions to the values on the new selected device.

The Reason specific Remote Map files can be found [here](https://github.com/JGuzak/MidiControllers/tree/master/RemoteMaps/C1Mono).

This device uses [hiduino](https://github.com/ddiakopoulos/hiduino), an open source firmware for Arduino controllers that makes computers recognize Arduinos as a standard usb-midi class compliant devices.