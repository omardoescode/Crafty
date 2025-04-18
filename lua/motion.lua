local maths = require("math")
local M = {}
M.args = {}

-- @field Value
-- @enum ValueType
M.move_steps = function(ctx)
	local chr = ctx.character
	local scope = ctx.scope

	local rotation = chr:rotation()
	local steps = scope:lookup_variable("steps"):tonumber()

	local x, y = chr:pos()
	-- TODO: Figure out solution to getter to pos other than
	local dx = maths.cos(rotation) * steps
	local dy = maths.sin(rotation) * steps
	chr:set_pos(math.floor(x + dx), math.floor(y + dy))

	return Value(ValueType.VOID)
end

M.go_to_x_y = function(ctx)
	local chr = ctx.character
	local scope = ctx.scope

	chr:set_pos(scope:lookup_variable("x"):tonumber(), scope:lookup_variable("y"):tonumber())

	return Value(ValueType.VOID)
end

M.turn_clockwise = function(ctx)
	local chr = ctx.character
	local scope = ctx.scope

	local new_rotation = scope:lookup_variable("degrees"):tonumber()
	local cur_rotation = chr:rotation()
	chr:set_rotation(cur_rotation - new_rotation)

	return Value(ValueType.VOID)
end

M.turn_anticlockwise = function(ctx)
	local chr = ctx.character
	local scope = ctx.scope

	local new_rotation = scope:lookup_variable("degrees"):tonumber()
	local cur_rotation = chr:rotation()
	chr:set_rotation(cur_rotation + new_rotation)

	return Value(ValueType.VOID)
end

M.point_direction = function(ctx)
	local chr = ctx.character
	local scope = ctx.scope

	local new_rotation = scope:lookup_variable("degrees"):tonumber()
	chr:set_rotation(new_rotation)

	return Value(ValueType.VOID)
end

M.set_x = function(ctx)
	local chr = ctx.character
	local scope = ctx.scope

	local x, y = chr:pos()
	local new_x = scope:lookup_variable("x"):tonumber()
	chr:set_pos(new_x, y)

	return Value(ValueType.VOID)
end

M.set_y = function(ctx)
	local chr = ctx.character
	local scope = ctx.scope

	local x, y = chr:pos()
	local new_y = scope:lookup_variable("y"):tonumber()
	chr:set_pos(x, new_y)

	return Value(ValueType.VOID)
end

M.x = function(ctx)
	local chr = ctx.character

	local ret = Value(ValueType.NUMBER)
	local x, y = chr:pos()
	ret:set(math.floor(x))
	return ret
end

M.y = function(ctx)
	local chr = ctx.character

	local ret = Value(ValueType.NUMBER)
	local x, y = chr:pos()
	ret:set(math.floor(y))
	return ret
end
return M
