# Arduino as ISP Shield

![Shield and ribbon](https://github.com/JGuzak/MidiControllers/blob/master/uno_isp/uno_isp_shield%20(1).jpg)

A simple shield made for icsp microcontroller programming. Works with all Arduino boards with icsp pin headers.

### First time setup:

1. If on windows, install Ubuntu for Windows. Information found [here](https://docs.microsoft.com/en-us/windows/wsl/install-win10)
2. In the example sketches provided in the Arduino IDE, upload 11.arduinoISP->arduinoISP sketch to the Uno acting as the programmer.

More detailed info on using an Arduino as ISP can be found [here](https://www.arduino.cc/en/Tutorial/ArduinoISP).

One thing to note, in the context of this project and the hiduino firmware, the target uno should be connected on the ICSP headers closest to the USB connector, not the ICSP header near the analog pins.

### Uploading new firmware:

![ISP programmer](https://github.com/JGuzak/MidiControllers/blob/master/uno_isp/uno_isp_shield%20(2).jpg)

1. Connect the isp shield to the Arduino that will be acting as the programmer.
2. Connect the ribbon cable from the isp shield to the icsp pin headers on the target Arduino.
3. Connect the isp Arduino via usb to your computer.
4. Open Bash or Terminal and navigate to "/MidiControllers/Controllers/avrdude-win-64bit/avrdude" in this repo. (If you modified and created a custom hiduino firmware, copy the .hex file here)

    Uploading usb-midi firmware:

    UNO: avrdude -c arduino -P COM4 -b 19200 -p m16u2 -vvv -U flash:w:arduino_midi.hex:i

    Uploading standard usb firmware:

    UNO: avrdude -c arduino -P COM4 -b 19200 -p m16u2 -vvv -U flash:w:usbserial_uno_16u2.hex:i
    DUE: avrdude -c arduino -P COM5 -b 19200 -p m16u2 -vvv -U flash:w:usbserial_due_16u2.hex:i

5. Modify -P parameter to match the isp Arduino COM port.
6. Run the modified command.

Back to [Main](https://github.com/JGuzak/MidiControllers)