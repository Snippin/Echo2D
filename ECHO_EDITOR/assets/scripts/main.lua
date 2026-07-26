-- Main lua script

entity = Entity("test", "grp")

local transform = entity:addComponent(
	Transform(-80, -80, 10, 10, 0)
)

local sprite = entity:addComponent(
	Sprite("hill_tiles", 16, 16, 1, 6, 0)
)

sprite:generateUVs()

local rotation = 0
local x = 10
local scale = 1
local move_right = true
local value = 0


main = {
	[1] = {
		Update = function()

		if rotation >= 360 then
			rotation = 0
		end

		if move_right and x < 150 then
			x = x + 3
			value = value + 0.01
		elseif move_right and x >= 150 then
			move_right = false
		end

		if not move_right and x > -150 then
			x = x - 3
			value = value - 0.01
		elseif not move_right and x <= -150 then
			move_right = true
		end

		local pos_x, pos_y = transform:Position()
		pos_x = x
		transform.Rotation = rotation
		transform:setPos(pos_x, pos_y)

		if move_right then
			rotation = rotation + 9
		else
			rotation = rotation - 9
		end

		scale = math.sin(value) * 10

		if scale >= 10 then
			scale = 10
		elseif scale <= 1 then
			scale = 1
		end

		if value >= 1 then
			value = 1
		elseif value <= 0 then
			value = 0
		end

		transform:setScale(scale, scale)

		end
	},
	[2] = {
		Render = function()
		
		end
	},
}