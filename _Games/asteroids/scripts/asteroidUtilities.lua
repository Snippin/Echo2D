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

function RemoveAsteroid(id)
    for key, value in pairs(asteroids) do
        if value.ID == id then
            if value.Type == "Big" then
                CreateSmallFromBigAsteroid(value)
                GAME_DATA:AddScore(BIG_ASTEROID_SCORE)
            elseif value.Type == "Small" then
                GAME_DATA:AddScore(SMALL_ASTEROID_SCORE)
            end

            local asteroid = Entity(value.ID)
            asteroid:Kill()
            asteroids[key] = nil
        end
    end
end

function UpdateAsteroids()
    for key, value in pairs(asteroids) do
        value:Update()
    end
end

function CreateSmallFromBigAsteroid(asteroid)
    local transform = Entity(asteroid.ID):GetComponent(Transform)
    for i = 1, 2 do
        local small = Asteroid:Create("small_asteroid")
        Entity(small.ID):GetComponent(Transform).Position = transform.Position
        AddAsteroid(small)
    end
end
