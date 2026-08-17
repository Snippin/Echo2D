Projectile = {}
Projectile.__index = Projectile

function Projectile:Create(params)
    local this = {
        Def = params.def,
        Dir = params.dir,
        StartPos = params.start_pos,
        Rotation = params.rotation,
    }

    local projectile = ProjectileDefinitions[this.Def]
    assert(projectile)

    this.ID = LoadEntity(projectile)
    this.Lifetime = projectile.lifetime or 2000
    this.Speed = projectile.speed or 10
    this.Timer = Timer()

    local entity = Entity(this.ID)
    local transform = entity:GetComponent(Transform)
    transform.Position = this.StartPos
    transform.Rotation = this.Rotation

    this.Timer:Start()

    setmetatable(this, self)
    return this
end

function Projectile:Update()
    local entity = Entity(self.ID)
    local transform = entity:GetComponent(Transform)
    transform.Position = transform.Position + (self.Dir * self.Speed)
end

function Projectile:IsLifetimeOver()
    return self.Timer:ElapsedMS() >= self.Lifetime
end

function Projectile:Destroy()
    local projectile = Entity(self.ID)
    projectile:Kill()
end
