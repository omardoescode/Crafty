local M = {}
M.args = {}

M.move_steps = function(ctx)
	-- Create a number value using the C++ Value class
	local num_value = Value(ValueType.NUMBER) -- Using the registered Value constructor
	num_value:set(42) -- Using the registered set method

	-- Create a text value using the C++ Value class
	local text_value = Value(ValueType.TEXT) -- Using the registered Value constructor
	text_value:set("Hello") -- Using the registered set method

	-- Add values to scope using the registered ScopeTable methods
	ctx.scope:add_variable("x", num_value)
	ctx.scope:add_variable("message", text_value)

	-- Look up values using the registered ScopeTable methods
	local x = ctx.scope:lookup_variable("x")
	local message = ctx.scope:lookup_variable("message")

	-- Get values using the conversion methods
	if x:type() == ValueType.NUMBER then -- Using the registered ValueType enum
		print("Number value:", x:tonumber()) -- Using the tonumber method
	end

	if message:type() == ValueType.TEXT then -- Using the registered ValueType enum
		print("Text value:", message:tostring()) -- Using the tostring method
	end
end

return M
