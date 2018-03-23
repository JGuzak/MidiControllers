function remote_init()
	local items={
		{name="Keyboard", input="keyboard"}
	}
	remote.define_items(items)

	local inputs={
		-- {pattern="b3 xx 7F", name="Keyboard", value="0", note="x", velocity="0"},
		-- {pattern="b3 xx 00", name="Keyboard", value="1", note="x", velocity="127"}
		{pattern="<100x>? yy zz", name="Keyboard"}

	}
	remote.define_auto_inputs(inputs)

end

function remote_probe()
	local controlRequest="F0 7E 7F 06 01 F7"
	local controlResponse="F0 7E 7F 06 02 00 01 79 03 00 01 00 20 13 05 30F7"
return {
		request=controlRequest,
		response=controlResponse,
	}
end