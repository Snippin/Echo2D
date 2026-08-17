Ship = {}
Ship.__index = Ship

function Ship:Create(def)
    local this = {
        ID = def.id,
        ForwardSpeed = def.forward_speed or 10,
        AngularSpeed = def.angular_speed or 4,
        DriftSpeed = def.drift_speed or 1,
        DriftAngle = Vec2(math.cos(0), math.sin(0)),
        StartDrifting = false,

        IsDead = false,
        Lives = GAME_DATA:GetNumLives(),
        Cooldown = def.cooldown or 200,

        CooldownTimer = Timer(),
        DeathTimer = Timer(),
        InvincibleTimer = Timer(),
    }

    setmetatable(this, self)
    return this
end

function Ship:Update()
    if self.IsDead then
        return
    end

    local ship = Entity(self.ID)
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

    if not self.CooldownTimer:IsRunning() then
        if Keyboard.Pressed(KEY_SPACE) then
            local projectile = Projectile:Create({
                def = "projectile1",
                dir = forward,
                start_pos = transform.Position,
                rotation = transform.Rotation,
            })
            AddPojectile(projectile)
            self.CooldownTimer:Start()
        end
    elseif self.CooldownTimer:ElapsedMS() >= self.Cooldown then
        self.CooldownTimer:Stop()
    end

    self:GetData()

    CheckPosition(transform.Position, transform.Scale,
        { x = sprite.Width, y = sprite.Height })
end

function Ship:GetData()
    if self.Lives ~= GAME_DATA:GetNumLives() then
        self.Lives = GAME_DATA:GetNumLives()
        self.InvincibleTimer:Start()
    end

    if self.InvincibleTimer:IsRunning() then
        local ship = Entity(self.ID)
        local sprite = ship:GetComponent(Sprite)
        sprite.Color.A = 150

        if self.InvincibleTimer:ElapsedMS() >= 3000 then
            local collider = ship:GetComponent(CircleCollider)
            collider.IsColliding = false
            sprite.Color.A = 255
            self.InvincibleTimer:Stop()
        end
    end
end
