-- Main lua script

entity = Entity("test", "grp")
local transform = entity:AddComponent(Transform(Vec2(-80), Vec2(10), 0))
local sprite = entity:AddComponent(Sprite("hill_tiles", 16, 16, 1, 6, 0))
sprite:GenerateUVs()

local rotation = 0
local x = 10
local scale = 1
local move_right = true
local value = 0

local get_name = entity:GetComponent(Sprite)
print("Tex Name: " ..get_name.Texture_Name)

entity2 = Entity("test2", "grp")
entity2:AddComponent(Transform(0, 0, 5, 5, 0))
local sprite2 = entity2:AddComponent(Sprite("warrior_princess", 16, 32, 0, 0, 0))
sprite2:GenerateUVs()

entity2:AddComponent(Animation(4, 5, 0, false))

local view = Registry.GetEntities(Transform)

view:Exclude(Sprite)

view:ForEach(
	function(entity)
		print(entity:Name())
	end
)

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

		transform.Position.x = x
		transform.Rotation = rotation

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

		transform.Scale = Vec2(scale)

		if Keyboard.JustPressed(KEY_Q) then
			print("Q just pressed")
		end

		if Keyboard.Pressed(KEY_W) then
			print("W pressed")
		end

		if Keyboard.JustReleased(KEY_E) then
			print("E just released")
		end

		end
	},
	[2] = {
		Render = function()

		end
	},
}