function remote_init()
	local items={
		{name="keyboard", input="keyboard"},
		{name="mod wheel", input="value", min=0, max=127},
		{name="pitch bend", input="value", min=0, max=16384},

		{name="button1", input="button", output="value"},
		{name="button2", input="button", output="value"},
		{name="button3", input="button", output="value"},
		{name="button4", input="button", output="value"},
		{name="button5", input="button", output="value"},
		{name="button6", input="button", output="value"},
		{name="button7", input="button", output="value"},
		{name="button8", input="button", output="value"},
		{name="button9", input="button", output="value"},
		{name="button10", input="button", output="value"},
		{name="button11", input="button", output="value"},
		{name="button12", input="button", output="value"},
		{name="button13", input="button", output="value"},
		{name="button14", input="button", output="value"},
		{name="button15", input="button", output="value"},
		{name="button16", input="button", output="value"},

		{name="rotary5", input="value", output="value", min=0, max=127},
		{name="rotary6", input="value", output="value", min=0, max=127},
		{name="rotary7", input="value", output="value", min=0, max=127},
		{name="rotary8", input="value", output="value", min=0, max=127},

		{name="rotary9", input="value", output="value", min=0, max=127},
		{name="rotary10", input="value", output="value", min=0, max=127},
		{name="rotary11", input="value", output="value", min=0, max=127},
		{name="rotary12", input="value", output="value", min=0, max=127},
		{name="rotary13", input="value", output="value", min=0, max=127},
		{name="rotary14", input="value", output="value", min=0, max=127},
		{name="rotary15", input="value", output="value", min=0, max=127},
		{name="rotary16", input="value", output="value", min=0, max=127}
	}
	remote.define_items(items)

	local inputs={
		{pattern="e0 xx yy", name="pitch bend", value="y*128 + x"},
		{pattern="b0 01 xx", name="mod wheel", value="x"},

		{pattern="80 xx yy", name="keyboard", value="0", note="x", velocity="100"},		
		{pattern="90 xx 00", name="keyboard", value="0", note="x", velocity="100"},
		{pattern="<100x>0 yy zz", name="keyboard"},

		{pattern="b0 14 xx", name="button1", value="1"},
		{pattern="b0 15 xx", name="button2", value="1"},
		{pattern="b0 16 xx", name="button3", value="1"},
		{pattern="b0 17 xx", name="button4", value="1"},
		{pattern="b0 18 xx", name="button5", value="1"},
		{pattern="b0 19 xx", name="button6", value="1"},
		{pattern="b0 1a xx", name="button7", value="1"},
		{pattern="b0 1b xx", name="button8", value="1"},
		{pattern="b0 1c xx", name="button9", value="1"},
		{pattern="b0 1d xx", name="button10", value="1"},
		{pattern="b0 1e xx", name="button11", value="1"},
		{pattern="b0 1f xx", name="button12", value="1"},
		{pattern="b0 20 xx", name="button13", value="1"},
		{pattern="b0 21 xx", name="button14", value="1"},
		{pattern="b0 22 xx", name="button15", value="1"},
		{pattern="b0 23 xx", name="button16", value="1"},

		{pattern="b0 6a xx", name="rotary5", value="x"},
		{pattern="b0 6b xx", name="rotary6", value="x"},
		{pattern="b0 6c xx", name="rotary7", value="x"},
		{pattern="b0 6d xx", name="rotary8", value="x"},

		{pattern="b0 6e xx", name="rotary9", value="x"},
		{pattern="b0 6f xx", name="rotary10", value="x"},
		{pattern="b0 70 xx", name="rotary11", value="x"},
		{pattern="b0 71 xx", name="rotary12", value="x"},
		{pattern="b0 72 xx", name="rotary13", value="x"},
		{pattern="b0 73 xx", name="rotary14", value="x"},
		{pattern="b0 74 xx", name="rotary15", value="x"},
		{pattern="b0 75 xx", name="rotary16", value="x"}
	}
	remote.define_auto_inputs(inputs)

	local outputs={
		{name="button1", pattern="b0 14 xx", x="value"},
		{name="button2", pattern="b0 15 xx", x="value"},
		{name="button3", pattern="b0 16 xx", x="value"},
		{name="button4", pattern="b0 17 xx", x="value"},
		{name="button5", pattern="b0 18 xx", x="value"},
		{name="button6", pattern="b0 19 xx", x="value"},
		{name="button7", pattern="b0 1a xx", x="value"},
		{name="button8", pattern="b0 1b xx", x="value"},
		{name="button9", pattern="b0 14 xx", x="value"},
		{name="button10", pattern="b0 15 xx", x="value"},
		{name="button11", pattern="b0 16 xx", x="value"},
		{name="button12", pattern="b0 17 xx", x="value"},
		{name="button13", pattern="b0 18 xx", x="value"},
		{name="button14", pattern="b0 19 xx", x="value"},
		{name="button15", pattern="b0 1a xx", x="value"},
		{name="button16", pattern="b0 1b xx", x="value"},

		{name="rotary5", pattern="b0 6a xx", x="value"},
		{name="rotary6", pattern="b0 6b xx", x="value"},
		{name="rotary7", pattern="b0 6c xx", x="value"},
		{name="rotary8", pattern="b0 6d xx", x="value"},

		{name="rotary9", pattern="b0 6e xx", x="value"},
		{name="rotary10", pattern="b0 6f xx", x="value"},
		{name="rotary11", pattern="b0 70 xx", x="value"},
		{name="rotary12", pattern="b0 71 xx", x="value"},
		{name="rotary13", pattern="b0 72 xx", x="value"},
		{name="rotary14", pattern="b0 73 xx", x="value"},
		{name="rotary15", pattern="b0 74 xx", x="value"},
		{name="rotary16", pattern="b0 75 xx", x="value"}
	}
	remote.define_auto_outputs(outputs)
	
end

function remote_probe()
	local controlRequest="F0 7E 7F 06 01 F7"
	local controlResponse="F0 7E 00 06 02 00 20 6B 02 00 04 ?? ?? ?? ?? ?? F7"
	return {
		request=controlRequest,
		response=controlResponse,
	}
end