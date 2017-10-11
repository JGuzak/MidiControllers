function remote_init()
	local items={

		{name="play", input="button", output="value", min=0, max=127},
		{name="pause", input="button", output="value", min=0, max=127},
		{name="record", input="button", output="value", min=0, max=127},

		{name="deck_a_encoder_1", input="delta"},
		{name="deck_a_encoder_button_1", input="button"},
		{name="deck_a_encoder_2", input="delta"},
		{name="deck_a_encoder_button_2", input="button"},

		{name="deck_a_shift_encoder_1", input="delta"},
		{name="deck_a_shift_encoder_button_1", input="button"},

		{name="deck_a_shift_encoder_2", input="delta"},
		{name="deck_a_shift_encoder_button_2", input="button"},

		{name="pad_1", input="button", output="value", min=0, max=127},
		{name="pad_2", input="button", output="value", min=0, max=127},
		{name="pad_3", input="button", output="value", min=0, max=127},
		{name="pad_4", input="button", output="value", min=0, max=127},
		{name="pad_5", input="button", output="value", min=0, max=127},
		{name="pad_6", input="button", output="value", min=0, max=127},
		{name="pad_7", input="button", output="value", min=0, max=127},
		{name="pad_8", input="button", output="value", min=0, max=127},

		{name="fader_1", input="value", output="value", min=0, max=127},
		{name="fader_2", input="value", output="value", min=0, max=127},
		{name="fader_3", input="value", output="value", min=0, max=127},
		{name="fader_4", input="value", output="value", min=0, max=127}

	}
	remote.define_items(items)

	local inputs={

		{pattern="b0 0a 7f", name="play", value="1"},
		{pattern="b0 09 7f", name="pause", value="1"},
		{pattern="b0 08 7f", name="record", value="1"},

		{pattern="b0 64 xx", name="deck_a_encoder_1", value="(64-x)*-1/63"},
		{pattern="b0 65 7f", name="deck_a_encoder_button_1", value="1"},
		{pattern="b0 15 xx", name="deck_a_encoder_2", value="(64-x)*1/63"},
		{pattern="b0 16 7f", name="deck_a_encoder_button_2", value="1"},

		{pattern="b0 78 xx", name="deck_a_shift_encoder_1", value="(64-x)*1/63"},
		{pattern="b0 79 7f", name="deck_a_shift_encoder_button_1", value="1"},
		{pattern="b0 33 xx", name="deck_a_shift_encoder_2", value="(64-x)*1/63"},
		{pattern="b0 34 7f", name="deck_a_shift_encoder_button_2", value="1"},

		{pattern="b0 0b 7f", name="pad_1", value="1"},
		{pattern="b0 0c 7f", name="pad_2", value="1"},
		{pattern="b0 0d 7f", name="pad_3", value="1"},
		{pattern="b0 0e 7f", name="pad_4", value="1"},
		{pattern="b0 0f 7f", name="pad_5", value="1"},
		{pattern="b0 10 7f", name="pad_6", value="1"},
		{pattern="b0 11 7f", name="pad_7", value="1"},
		{pattern="b0 12 7f", name="pad_8", value="1"},

		{pattern="b0 54 xx", name="fader_1", value="x"},
		{pattern="b0 55 xx", name="fader_2", value="x"},
		{pattern="b0 56 xx", name="fader_3", value="x"},
		{pattern="b0 57 xx", name="fader_4", value="x"}
		
	}
	remote.define_auto_inputs(inputs)

	local outputs={

		{name="play", pattern="b0 0a xx", x="value"},
		{name="pause", pattern="b0 09 xx", x="value"},
		{name="record", pattern="b0 08 xx", x="value"},

		{name="pad_1", pattern="b0 0f xx", x="value"},
		{name="pad_1", pattern="b0 0b xx", x="value"},
		{name="pad_2", pattern="b0 10 xx", x="value"},
		{name="pad_2", pattern="b0 0c xx", x="value"},
		{name="pad_3", pattern="b0 11 xx", x="value"},
		{name="pad_3", pattern="b0 0d xx", x="value"},
		{name="pad_4", pattern="b0 12 xx", x="value"},
		{name="pad_4", pattern="b0 0e xx", x="value"},

		{name="fader_1", pattern="b0 54 xx", x ="value"},
		{name="fader_2", pattern="b0 55 xx", x ="value"},
		{name="fader_3", pattern="b0 56 xx", x ="value"},
		{name="fader_4", pattern="b0 57 xx", x ="value"}
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