local M = {}

M.run_clicked = function(ctx) end

M.signal_received = function(ctx)
	if not ctx then
		print("[ERROR] ctx is nil in signal_received!")
		return
	end
	local scope = ctx.scope

	local signal = scope:lookup_variable("signal"):tostring()
	debug_args(signal, -1)
	wait_for_event(signal, -1) -- TODO: Complete this
end

M.send_signal = function(ctx)
	local scope = ctx.scope

	local signal = scope:lookup_variable("signal"):tostring()
	emit_event(signal)
end
M.wait = function(ctx)
	local scope = ctx.scope

	local seconds = scope:lookup_variable("seconds"):tonumber()
	wait(seconds)
end
return M
