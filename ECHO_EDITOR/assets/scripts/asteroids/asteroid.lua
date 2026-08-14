Asteroid = {}
Asteroid.__index = Asteroid

function Asteroid:Create(def)
    local asteroid = AsteroidDefinitions[def]
    assert(asteroid, "Asteroid definition does not exist")

    local this = {
        Def = def,
        ID = -1,
        Type = asteroid.type,
        MinSpeed = asteroid.min_speed,
        MaxSpeed = asteroid.max_speed,
        Velocity = GetRandomVelocity(asteroid.min_speed, asteroid.max_speed),
        RotationDir = math.random(-1, 1),
        RotationSpeed = math.random(1, 4),
    }

    this.ID = LoadEntity(asteroid)

    local entity = Entity(this.ID)
    local transform = entity:GetComponent(Transform)
    transform.Position = GetRandomPosition()

    setmetatable(this, self)
    return this
end

function Asteroid:Update()
    local entity = Entity(self.ID)
    local transform = entity:GetComponent(Transform)
    local sprite = entity:GetComponent(Sprite)

    transform.Position = transform.Position + self.Velocity
    transform.Rotation = transform.Rotation + self.RotationSpeed * self.RotationDir

    CheckPosition(transform.Position, transform.Scale,
        { x = sprite.Width, y = sprite.Height })
end
