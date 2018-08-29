# Controllers

## C1Mono

![protoboard image](https://github.com/JGuzak/MidiControllers/blob/master/Controllers/C1Mono/proto%20C1%20for%20uno/protoboard_mount_1.jpg)

Uses [this](https://github.com/JGuzak/MidiControllers/tree/master/RemoteMaps/C1Mono) Remote Map for controlling [Reason 10](https://www.propellerheads.se/en/reason). Currently designing and testing pcb and case options for the final product.

More details [here](https://github.com/JGuzak/MidiControllers/tree/master/Controllers/C1Mono)

## Hiduino

Original [repo](https://github.com/ddiakopoulos/hiduino)

### Setup environment for making custom named hiduino firmware (Windows 10)

1. Install Ubuntu for Windows. Information found [here](https://docs.microsoft.com/en-us/windows/wsl/install-win10)
2. Run these commands:
    * sudo apt-get update
    * sudo apt-get install avr-libc avrdude binutils-avr gcc-avr srecord
3. Download LUFA packages from [here](http://www.fourwalledcubicle.com/LUFA.php)

### Making a new Hiduino Firmware

1. Copy "Controllers/Hiduino/src/arduino_midi" folder to the working directory of the desired controller
2. Rename "arduino_midi" to "hiduino_{name of the controller}"
3. Inside "hiduino_{name of the controller}" modify the following:
    * Descriptors.h line 263: change arduino_midi to the desired usb device name
    * Makefile line 22: set to the proper relative path for the LUFA packages
4. In bash, run the command "make" inside the "hiduino_{name of the controller}" directory. The {name of the controller}.hex file is the target for uploading to the usb micro controller on the Arduino.
5. Info on uploading firmware via an Arduino Uno as ISP can be found [here](https://github.com/JGuzak/MidiControllers/blob/master/uno_isp/README.md)

Back to [Main](https://github.com/JGuzak/MidiControllers)
