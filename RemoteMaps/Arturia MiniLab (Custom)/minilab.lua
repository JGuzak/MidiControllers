function remote_init()
	local items={
		{name="keyboard", input="keyboard"},
		{name="pitch bend", input="value", min=0, max=16384},
		{name="mod wheel", input="value", min=0, max=127},
		{name="damper pedal", input="button"},

		{name="encoder1", input="delta"},
		{name="encoder2", input="delta"},
		{name="encoder3", input="delta"},
		{name="encoder4", input="delta"},
		{name="encoder5", input="delta"},
		{name="encoder6", input="delta"},
		{name="encoder7", input="delta"},
		{name="encoder8", input="delta"},

		{name="encoder9", input="delta"},
		{name="encoder10", input="delta"},
		{name="encoder11", input="delta"},
		{name="encoder12", input="delta"},
		{name="encoder13", input="delta"},
		{name="encoder14", input="delta"},
		{name="encoder15", input="delta"},
		{name="encoder16", input="delta"},

		{name="button1", input="button"},
		{name="button2", input="button"},
		{name="button3", input="button"},
		{name="button4", input="button"},
		{name="button5", input="button"},
		{name="button6", input="button"},
		{name="button7", input="button"},
		{name="button8", input="button"},

		{name="button9", input="button"},
		{name="button10", input="button"},
		{name="button11", input="button"},
		{name="button12", input="button"},
		{name="button13", input="button"},
		{name="button14", input="button"},
		{name="button15", input="button"},
		{name="button16", input="button"}

			}
	remote.define_items(items)

	local inputs={
		{pattern="80 xx yy", name="keyboard", value="0", note="x", velocity="100"},
		{pattern="90 xx 00", name="keyboard", value="0", note="x", velocity="100"},
		{pattern="<100x>0 yy zz", name="keyboard"},

		{pattern="e0 xx yy", name="pitch bend", value="y*128 + x"},
		{pattern="b0 01 xx", name="mod wheel"},
		{pattern="b0 40 xx", name="damper pedal", value="1"},

		{pattern="b0 14 41", name="encoder1", value="1"},
		{pattern="b0 14 3f", name="encoder1", value="-1"},
		{pattern="b0 15 41", name="encoder2", value="1"},
		{pattern="b0 15 3f", name="encoder2", value="-1"},
		{pattern="b0 16 41", name="encoder3", value="1"},
		{pattern="b0 16 3f", name="encoder3", value="-1"},
		{pattern="b0 17 41", name="encoder4", value="1"},
		{pattern="b0 17 3f", name="encoder4", value="-1"},
		{pattern="b0 18 41", name="encoder5", value="1"},
		{pattern="b0 18 3f", name="encoder5", value="-1"},
		{pattern="b0 19 41", name="encoder6", value="1"},
		{pattern="b0 19 3f", name="encoder6", value="-1"},
		{pattern="b0 1a 41", name="encoder7", value="1"},
		{pattern="b0 1a 3f", name="encoder7", value="-1"},
		{pattern="b0 1b 41", name="encoder8", value="1"},
		{pattern="b0 1b 3f", name="encoder8", value="-1"},

		{pattern="b0 1c 41", name="encoder9", value="1"},
		{pattern="b0 1c 3f", name="encoder9", value="-1"},
		{pattern="b0 1d 41", name="encoder10", value="1"},
		{pattern="b0 1d 3f", name="encoder10", value="-1"},
		{pattern="b0 1e 41", name="encoder11", value="1"},
		{pattern="b0 1e 3f", name="encoder11", value="-1"},
		{pattern="b0 1f 41", name="encoder12", value="1"},
		{pattern="b0 1f 3f", name="encoder12", value="-1"},
		{pattern="b0 20 41", name="encoder13", value="1"},
		{pattern="b0 20 3f", name="encoder13", value="-1"},
		{pattern="b0 21 41", name="encoder14", value="1"},
		{pattern="b0 21 3f", name="encoder14", value="-1"},
		{pattern="b0 22 41", name="encoder15", value="1"},
		{pattern="b0 22 3f", name="encoder15", value="-1"},
		{pattern="b0 23 41", name="encoder16", value="1"},
		{pattern="b0 23 3f", name="encoder16", value="-1"},

		{pattern="90 24 xx", name="button1", value="1"},
		{pattern="80 24 xx", name="button1", value="0"},
		{pattern="90 25 xx", name="button2", value="1"},
		{pattern="80 25 xx", name="button2", value="0"},
		{pattern="90 26 xx", name="button3", value="1"},
		{pattern="80 26 xx", name="button3", value="0"},
		{pattern="90 27 xx", name="button4", value="1"},
		{pattern="80 27 xx", name="button4", value="0"},
		{pattern="90 28 xx", name="button5", value="1"},
		{pattern="80 28 xx", name="button5", value="0"},
		{pattern="90 29 xx", name="button6", value="1"},
		{pattern="80 29 xx", name="button6", value="0"},
		{pattern="90 2a xx", name="button7", value="1"},
		{pattern="80 2a xx", name="button7", value="0"},
		{pattern="90 2b xx", name="button8", value="1"},
		{pattern="80 2b xx", name="button8", value="0"},

		{pattern="b0 34 7f", name="button9", value="1"},
		{pattern="b0 35 xx", name="button10", value="1"},
		{pattern="b0 36 xx", name="button11", value="1"},
		{pattern="b0 37 xx", name="button12", value="1"},
		{pattern="b0 38 xx", name="button13", value="1"},
		{pattern="b0 39 xx", name="button14", value="1"},
		{pattern="b0 3a xx", name="button15", value="1"},
		{pattern="b0 3b xx", name="button16", value="1"}

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

