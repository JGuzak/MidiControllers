# Remote Maps

Remote is a Lua based scripting protocol that handles communication between midi controllers and Reason devices/session parameters. This protocol allows midi controllers to automatically map to various parameters based on the selected device to reduce the need to manual map hardware controls to software controls.

Propellerheads Remote [Web page](https://www.propellerheads.se/support/user/reason/remote/)

## Installing new Remote Scripts

A Remote Script contains these four files:
    {ControllerName}.lua
    {ControllerName}.luacodec
    {ControllerName}.png
    {ControllerName}.remotemap

1. (On Windows) Navigate to C:/ProgramData/Properllerhead Software/Remote/ or (On Mac) Navigate to Library:Application Support:Propellerhead Software:Remote:
2. The .lua, .luacodec, and .png files all go inside Codecs/ControllerCompanyName/ folder.
3. The .remotemap file goes inside the Maps/ControllerCompanyName/ folder. Side Note: If these folders don't exist inside Codecs/ and Maps/, make them with the same name as the manufacturer specified inside the .luacodec file on line 9.
4. Connect the controller to the computer.
5. Launch Reason and open the Preferences window.
6. Navigate to the Control Surfaces tab then: Add->Select the Manufacturer name->Select the device->Select midi source from connected devices->Hit ok and you are set!

### Correct File Structure

    Remote/
        Codecs/
            ControllerCompanyName/
                {ControllerName}.lua
                {ControllerName}.luacodec
                {ControllerName}.png
        Maps/
            ControllerCompanyName/
                {ControllerName}.remotemap

Back to [Main](https://github.com/JGuzak/MidiControllers)