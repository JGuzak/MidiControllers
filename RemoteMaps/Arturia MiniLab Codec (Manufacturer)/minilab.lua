function remote_init()
	local items={
		{name="Keyboard", input="keyboard"},
		{name="Mod Wheel", input="value", min=0, max=127},
		{name="Pitch Bend", input="value", min=0, max=16384},
		{name="Damper Pedal", input="button"},

		{name="click", input="button"},
		{name="quant", input="button"},
		{name="rew", input="button"},
		{name="fwd", input="button"},
		{name="stop", input="button"},
		{name="play", input="button"},
		{name="record", input="button"},
		{name="loop", input="button"},

		{name="volume", input="delta"},
		{name="cut", input="delta"},
		{name="reso", input="delta"},
		{name="lforate", input="delta"},
		{name="lfoamt", input="delta"},
		{name="chorus", input="delta"},
		{name="atk1", input="delta"},
		{name="dec1", input="delta"},

		{name="preset", input="delta"},
		{name="param1", input="delta"},
		{name="param2", input="delta"},
		{name="param3", input="delta"},
		{name="param4", input="delta"},
		{name="delay", input="delta"},
		{name="sus1", input="delta"},
		{name="rel1", input="delta"},		

			}
	remote.define_items(items)

	local inputs={

		{pattern="b0 16 xx", name="click", value="1"},
		{pattern="b0 17 xx", name="quant", value="1"},
		
		{pattern="F0 7F 7F 06 01 F7", name="stop", value="1"},
		{pattern="F0 7F 7F 06 02 F7", name="play", value="1"},
		{pattern="F0 7F 7F 06 05 F7", name="rew", value="1"},
		{pattern="F0 7F 7F 06 04 F7", name="fwd", value="1"},
		{pattern="b0 1D xx", name="loop", value="1"},
		{pattern="b0 1C xx", name="record", value="1"},

		{pattern="b0 07 xx", name="volume", value="(64 - x) * -1"},
		{pattern="b0 4A xx", name="cut", value="(64 - x) * -1"},
		{pattern="b0 47 xx", name="reso", value="(64 - x) * -1"},
		{pattern="b0 4C xx", name="lforate", value="(64 - x) * -1"},
		{pattern="b0 4D xx", name="lfoamt", value="(64 - x) * -1"},
		{pattern="b0 5D xx", name="chorus", value="(64 - x) * -1"},
		{pattern="b0 49 xx", name="atk1", value="(64 - x) * -1"},
		{pattern="b0 4B xx", name="dec1", value="(64 - x) * -1"},

		{pattern="b0 72 xx", name="preset", value="(64 - x) * -1"},
		{pattern="b0 12 xx", name="param1", value="(64 - x) * -1"},
		{pattern="b0 13 xx", name="param2", value="(64 - x) * -1"},
		{pattern="b0 10 xx", name="param3", value="(64 - x) * -1"},
		{pattern="b0 11 xx", name="param4", value="(64 - x) * -1"},
		{pattern="b0 5B xx", name="delay", value="(64 - x) * -1"},
		{pattern="b0 4F xx", name="sus1", value="(64 - x) * -1"},
		{pattern="b0 48 xx", name="rel1", value="(64 - x) * -1"},

		{pattern="e0 xx yy", name="Pitch Bend", value="y*128 + x"},
		{pattern="b0 01 xx", name="Mod Wheel"},
		{pattern="b0 40 xx", name="Damper Pedal", value="1"},

		{pattern="80 xx yy", name="Keyboard", value="0", note="x", velocity="64"},		
		{pattern="90 xx 00", name="Keyboard", value="0", note="x", velocity="64"},
		{pattern="<100x>0 yy zz", name="Keyboard"},

	
	}
	remote.define_auto_inputs(inputs)
end


function remote_probe()

	local controlRequest="F0 7E 7F 06 01 F7"

	local controlResponse="F0 7E 00 06 02 00 20 6B 02 00 04 ?? ?? ?? ?? ?? F7"
return {
		request=controlRequest,
		response=controlResponse,
	}
end

