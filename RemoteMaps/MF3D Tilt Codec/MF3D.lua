function remote_init()
	local items={
		{name="keyboard", input="keyboard"},

		{name="leftTilt", input="value", min=0, max=127},
		{name="rightTilt", input="value", min=0, max=127},
		{name="frontTilt", input="value", min=0, max=127},
		{name="backTilt", input="value", min=0, max=127}

	}
	remote.define_items(items)

	local inputs={
		{pattern="<100x>0 yy zz", name="keyboard"},

		{pattern="b1 00 xx", name="leftTilt", value="x"},
		{pattern="b1 02 xx", name="rightTilt", value="x"},
		{pattern="b1 01 xx", name="frontTilt", value="x"},
		{pattern="b1 03 xx", name="backTilt", value="x"}

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