local M = {}
M.set_string = function(ctx)
	local var = ctx.scope:lookup_variable("variable")
	local val = ctx.scope:lookup_variable("value")
	ctx.scope:add_variable(var:tostring(), val)

	return Value(ValueType.VOID)
end
M.set_string_global = function(ctx)
	local var = ctx.scope:lookup_variable("variable")
	local val = ctx.scope:lookup_variable("value")
	ctx.scope:add_variable_global(var:tostring(), val)

	return Value(ValueType.VOID)
end

M.set_number = function(ctx)
	local var = ctx.scope:lookup_variable("variable")
	local val = ctx.scope:lookup_variable("value")
	ctx.scope:add_variable(var:tostring(), val)

	return Value(ValueType.VOID)
end

M.set_number_global = function(ctx)
	local var = ctx.scope:lookup_variable("variable")
	local val = ctx.scope:lookup_variable("value")
	ctx.scope:add_variable_global(var:tostring(), val)

	return Value(ValueType.VOID)
end
return M
