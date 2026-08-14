-- Main lua script

run_script("assets/scripts/asteroids/entityDefinitions.lua")
run_script("assets/scripts/asteroids/assetDefinitions.lua")
run_script("assets/scripts/asteroids/utilities.lua")
run_script("assets/scripts/asteroids/ship.lua")
run_script("assets/scripts/asteroids/asteroid.lua")
run_script("assets/scripts/asteroids/asteroidUtilities.lua")
run_script("assets/scripts/asteroids/collisionSystem.lua")
run_script("assets/scripts/asteroids/projectile.lua")
run_script("assets/scripts/asteroids/projectileUtilities.lua")

math.randomseed(os.time())
LoadAssets()
LoadBackground()

local ship = LoadEntity(ShipDefinitions["base_ship"])
gShip = Ship:Create({ id = ship })

gCollisionSystem = CollisionSystem:Create()

main = {
    [1] = {
        Update = function()
            SpawnAsteroids()

            gShip:Update()
            UpdateAsteroids()
            UpdateProjectiles()
            gCollisionSystem:Update()
        end
    },
    [2] = {
        Render = function()

        end
    },
}
