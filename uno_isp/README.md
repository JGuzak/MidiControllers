# Arduino as ISP Shield

![Shield and ribbon](https://github.com/JGuzak/MidiControllers/blob/master/uno_isp/uno_isp_shield%20(1).jpg)

A simple shield made for icsp microcontroller programming. Works with all Arduino boards with icsp pin headers.

### Setup:
 1.
 2.
 3.
 4.
 5.

### Uploading new firmware:

![ISP programming](https://github.com/JGuzak/MidiControllers/blob/master/uno_isp/uno_isp_shield%20(2).jpg)

    1. Connect the isp shield to the Arduino that will be acting as the programmer.
    2. Connect the ribbon cable from the isp shield to the icsp pin headers on the target Arduino.
    3. Connect the isp Arduino via usb to your computer.
    3. Open Command Prompt or Terminal and navigate to "/MidiControllers/Controllers/avrdude-win-64bit/avrdude" in this repo.

    Uploading usb-midi firmware:

    UNO: avrdude -c arduino -P COM4 -b 19200 -p m16u2 -vvv -U flash:w:arduino_midi.hex:i

    Uploading standard usb firmware:

    UNO: avrdude -c arduino -P COM4 -b 19200 -p m16u2 -vvv -U flash:w:usbserial_uno_16u2.hex:i
    DUE: avrdude -c arduino -P COM5 -b 19200 -p m16u2 -vvv -U flash:w:usbserial_due_16u2.hex:i

    5. Modify -P parameter to match the isp Arduino COM port.
    6. Run the modified command.
