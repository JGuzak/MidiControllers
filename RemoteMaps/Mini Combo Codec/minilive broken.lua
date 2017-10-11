function remote_init()
	local items={
		{name="keyboard", input="keyboard"},
		{name="mod wheel", input="value", min=0, max=127},
		{name="pitch bend", input="value", min=0, max=16384},

		{name="play", input="button"},
		{name="stop", input="button"},
		{name="click", input="button"},
		{name="record", input="button"},

		{name="button1", input="button"},
		{name="button2", input="button"},
		{name="button3", input="button"},
		{name="button4", input="button"},

		{name="encoder1", input="delta"}
		--{name="encoder16", input="delta", output="value", min=0, max=127}
	}
	remote.define_items(items)

	local inputs={
		{pattern="f0 7f 7f 06 02 f7", name="play", value="1"},
		{pattern="f0 7f 7f 06 01 f7", name="stop", value="1"},
		{pattern="f0 7f 7f 06 05 f7", name="click", value="1"},
		{pattern="f0 7f 7f 06 08 f7", name="record", value="1"},

		{pattern="b0 10 xx", name="button1", value="1"},
		{pattern="b0 11 xx", name="button2", value="1"},
		{pattern="b0 12 xx", name="button3", value="1"},
		{pattern="b0 13 xx", name="button4", value="1"},

		{pattern="b1 66 3f", name="encoder1", value="-1"},
		{pattern="b1 66 41", name="encoder1", value="1"},

		{pattern="e0 xx yy", name="pitch bend", value="y*128 + x"},
		{pattern="b0 01 xx", name="mod wheel", value="x"},

		{pattern="80 xx yy", name="keyboard", value="0", note="x", velocity="100"},		
		{pattern="90 xx 00", name="keyboard", value="0", note="x", velocity="100"},
		{pattern="<100x>0 yy zz", name="keyboard"}
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

MAX_INDEX = 27
INDEX_OFFSET = 11
CC_OFFSET = 66

MIN_CC_ID = 67
MAX_CC_ID = 75

-- the value the encoder sends out on a null signal
DELTA_ZERO = 65


MIN_VAL = 0
MAX_VAL = 127

-- stores knob values 
--		use offsets
knob_vals = {}

function remote_process_midi(event)
	
	-- handles delta values
	delta_cc = remote.match_midi("b0 yy <???x>?", event)
	if delta_cc ~= nil then
		-- checks midi cc id within proper range
		if MIN_CC_ID <= delta_cc.y and delta_cc.y <= MAX_CC_ID then
			index = CC_OFFSET-delta_cc.y
			-- determine delta amount
			if delta_cc.x > DELTA_ZERO then
				delta = 1
			end
			if delta_cc.x < DELTA_ZERO then
				delta = -1
			end
			delta = 0
			
			-- store new value in knob_vals if value will be in range
			value = knob_vals[index]
			if MIN_VAL <= value+delta and value+delta <= MAX_VAL then
				knob_vals[index] = value + delta
			end
		end
	end
end

function handle_input(msg)

	if msg.item >= INDEX_OFFSET then
		remote.make_midi("b1 yy xx", {y="", x="knob_vals[msg.item-INDEX_OFFSET]"})
	end
end

function remote_set_state(changed_items)
	for i, item_index in pairs(changed_items) do
		if 0 < item_index-INDEX_OFFSET then
			knob_vals[item_index-INDEX_OFFSET] = changed_items[item_index]
		end
	end
end