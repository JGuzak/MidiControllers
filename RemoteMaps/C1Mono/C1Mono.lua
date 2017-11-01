function remote_init()
	local items={

		{name="rotary1", input="value", output="value", min=0, max=127},
		{name="rotary2", input="value", output="value", min=0, max=127},
		{name="rotary3", input="value", output="value", min=0, max=127},
		{name="rotary4", input="value", output="value", min=0, max=127},
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
		{name="rotary16", input="value", output="value", min=0, max=127},

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
		
		{name="deltaRotary", input="delta"}

	}
	remote.define_items(items)

	local inputs={

		{pattern="b0 0A xx", name="rotary1", value="x"},
		{pattern="b0 0B xx", name="rotary2", value="x"},
		{pattern="b0 0C xx", name="rotary3", value="x"},
		{pattern="b0 0D xx", name="rotary4", value="x"},
		{pattern="b0 0E xx", name="rotary5", value="x"},
		{pattern="b0 0F xx", name="rotary6", value="x"},
		{pattern="b0 10 xx", name="rotary7", value="x"},
		{pattern="b0 11 xx", name="rotary8", value="x"},
		{pattern="b0 12 xx", name="rotary9", value="x"},
		{pattern="b0 13 xx", name="rotary10", value="x"},
		{pattern="b0 14 xx", name="rotary11", value="x"},
		{pattern="b0 15 xx", name="rotary12", value="x"},
		{pattern="b0 16 xx", name="rotary13", value="x"},
		{pattern="b0 17 xx", name="rotary14", value="x"},
		{pattern="b0 18 xx", name="rotary15", value="x"},
		{pattern="b0 19 xx", name="rotary16", value="x"},

		{pattern="b0 1A xx", name="button1", value="1"},
		{pattern="b0 1B xx", name="button2", value="1"},
		{pattern="b0 1C xx", name="button3", value="1"},
		{pattern="b0 1D xx", name="button4", value="1"},
		{pattern="b0 1E xx", name="button5", value="1"},
		{pattern="b0 1F xx", name="button6", value="1"},
		{pattern="b0 20 xx", name="button7", value="1"},
		{pattern="b0 21 xx", name="button8", value="1"},
		{pattern="b0 22 xx", name="button9", value="1"},
		{pattern="b0 23 xx", name="button10", value="1"},
		{pattern="b0 24 xx", name="button11", value="1"},
		{pattern="b0 25 xx", name="button12", value="1"},
		{pattern="b0 26 xx", name="button13", value="1"},
		{pattern="b0 27 xx", name="button14", value="1"},
		{pattern="b0 28 xx", name="button15", value="1"},
		{pattern="b0 29 xx", name="button16", value="1"},

		{pattern="b0 2A xx", name="deltaRotary", value="(64-x)"}
	
	}
	remote.define_auto_inputs(inputs)

	local outputs={

		{name="rotary1", pattern="b0 0A xx"},
		{name="rotary2", pattern="b0 0B xx"},
		{name="rotary3", pattern="b0 0C xx"},
		{name="rotary4", pattern="b0 0D xx"},
		{name="rotary5", pattern="b0 0E xx"},
		{name="rotary6", pattern="b0 0F xx"},
		{name="rotary7", pattern="b0 10 xx"},
		{name="rotary8", pattern="b0 11 xx"},
		{name="rotary9", pattern="b0 12 xx"},
		{name="rotary10", pattern="b0 13 xx"},
		{name="rotary11", pattern="b0 14 xx"},
		{name="rotary12", pattern="b0 15 xx"},
		{name="rotary13", pattern="b0 16 xx"},
		{name="rotary14", pattern="b0 17 xx"},
		{name="rotary15", pattern="b0 18 xx"},
		{name="rotary16", pattern="b0 19 xx"},

		{name="button1", pattern="b0 1A 0<000x>"},
		{name="button2", pattern="b0 1B 0<000x>"},
		{name="button3", pattern="b0 1C 0<000x>"},
		{name="button4", pattern="b0 1D 0<000x>"},
		{name="button5", pattern="b0 1E 0<000x>"},
		{name="button6", pattern="b0 1F 0<000x>"},
		{name="button7", pattern="b0 20 0<000x>"},
		{name="button8", pattern="b0 21 0<000x>"},
		{name="button9", pattern="b0 22 0<000x>"},
		{name="button10", pattern="b0 23 0<000x>"},
		{name="button11", pattern="b0 24 0<000x>"},
		{name="button12", pattern="b0 25 0<000x>"},
		{name="button13", pattern="b0 26 0<000x>"},
		{name="button14", pattern="b0 27 0<000x>"},
		{name="button15", pattern="b0 28 0<000x>"},
		{name="button16", pattern="b0 29 0<000x>"}

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