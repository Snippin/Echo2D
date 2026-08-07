local spawnTimer = Timer()
local asteroids = {}

function SpawnAsteroids()
    spawnTimer:Start()

    if spawnTimer:ElapsedSec() > 2 then
        local val = math.random(1, 3)
        if val == 1 then
            local asteroid = Asteroid:Create("big_asteroid")
            AddAsteroid(asteroid)
        elseif val == 2 then
            local asteroid = Asteroid:Create("small_asteroid")
            AddAsteroid(asteroid)
        elseif val == 3 then
            -- TODO : Create enemy ship?
        end

        spawnTimer:Stop()
    end
end

function AddAsteroid(asteroid)
    table.insert(asteroids, asteroid)
end

function UpdateAsteroids()
    for key, value in pairs(asteroids) do
        value:Update()
    end
end
