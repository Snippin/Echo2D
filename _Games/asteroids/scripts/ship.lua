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

        IsExploding = false,
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

    if self.IsExploding then
        self:UpdateExplosion()
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
        self.StartDrifting = true
    elseif not Keyboard.Pressed(KEY_W) and self.StartDrifting then
        transform.Position =
            transform.Position + self.DriftAngle * self.DriftSpeed
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

    self:UpdateData()

    CheckPosition(transform.Position, transform.Scale,
        { x = sprite.Width, y = sprite.Height })
end

function Ship:UpdateData()
    local lives = GAME_DATA:GetNumLives()

    if self.Lives ~= lives and not self.IsExploding then
        self.IsExploding = true

        local ship = Entity(self.ID)
        local transform = ship:GetComponent(Transform)
        local sprite = ship:GetComponent(Sprite)
        local animation = ship:GetComponent(Animation)

        animation.Frames = 8
        animation.FrameRate = 16
        animation:Restart()

        sprite.Texture_Name = "explosion"
        sprite.Width = 34
        sprite.Height = 34
        sprite:GenerateUVs()

        transform.Scale = Vec2(2)

        self.Lives = lives
        self.DeathTimer:Start()
    end

    if self.InvincibleTimer:IsRunning() then
        local ship = Entity(self.ID)
        local sprite = ship:GetComponent(Sprite)
        sprite.Color.A = 150

        if self.InvincibleTimer:ElapsedMS() >= 3000 then
            local collider = ship:GetComponent(CircleCollider)
            collider.IsColliding = false

            self.InvincibleTimer:Stop()
            sprite.Color.A = 255
        end
    end
end

function Ship:UpdateExplosion()
    if self.IsExploding and self.DeathTimer:ElapsedMS() >= 1000 then
        local ship = Entity(self.ID)
        local animation = ship:GetComponent(Animation)

        if animation.CurrentFrame >= animation.Frames - 1 then
            -- Reset animation
            animation.Frames = 0
            animation.FrameRate = 0
            animation.CurrentFrame = 0

            -- Reset sprite to ship
            local sprite = ship:GetComponent(Sprite)
            sprite.Texture_Name = "ship"
            sprite.Width = 99
            sprite.Height = 75
            sprite:GenerateUVs()
            sprite.Start_X = 0

            -- Reset scale
            local transform = ship:GetComponent(Transform)
            transform.Scale = Vec2(1)

            -- Reset explosion
            self.IsExploding = false
            self.DeathTimer:Stop()
            self.InvincibleTimer:Start()

            if GAME_DATA:GetNumLives() == 0 then
                if not self.IsDead then
                    sprite.IsHidden = true
                end

                self.IsDead = true
            end
        end
    end
end

function Ship:Reset()
    self.StartDrifting = false
    self.IsExploding = false
    self.IsDead = false
    self.Lives = GAME_DATA:GetNumLives()
    self.DeathTimer:Stop()
    self.InvincibleTimer:Stop()
    self.CooldownTimer:Stop()

    local ship = Entity(self.ID)

    local transform = ship:GetComponent(Transform)
    transform.Position = Vec2(0)
    transform.Rotation = -90.0

    local sprite = ship:GetComponent(Sprite)
    sprite.IsHidden = false
    sprite.Color.A = 255

    local collider = ship:GetComponent(CircleCollider)
    collider.IsColliding = false
end
