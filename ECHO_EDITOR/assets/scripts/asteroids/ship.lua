Ship = {}
Ship.__index = Ship

function Ship:Create(def)
    local this = {
        EntityID = def.id,
        ForwardSpeed = def.forward_speed or 10,
        AngularSpeed = def.angular_speed or 4,
        DriftSpeed = def.drift_speed or 1,
        DriftAngle = Vec2(math.cos(0), math.sin(0)),
        StartDrifting = false,
    }

    setmetatable(this, self)
    return this
end

function Ship:UpdateShip()
    local ship = Entity(self.EntityID)
    local transform = ship:GetComponent(Transform)
    local sprite = ship:GetComponent(Sprite)

    local forward = Vec2(
        math.cos(math.rad(transform.Rotation)),
        math.sin(math.rad(transform.Rotation))
    )

    if Keyboard.Pressed(KEY_A) then
        transform.Rotation = transform.Rotation - self.AngularSpeed
    elseif Keyboard.Pressed(KEY_D) then
        transform.Rotation = transform.Rotation + self.AngularSpeed
    end

    if Keyboard.Pressed(KEY_W) then
        transform.Position = transform.Position + forward * self.ForwardSpeed
        self.DriftAngle = forward
        self.StartDrifitng = true
    elseif not Keyboard.Pressed(KEY_W) and self.StartDrifitng then
        transform.Position = transform.Position + self.DriftAngle * self.DriftSpeed
    end

    CheckPosition(transform.Position,
        sprite.Width * 0.5 * transform.Scale.x,
        sprite.Height * 0.5 * transform.Scale.y)
end
