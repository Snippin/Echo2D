-- Main lua script

run_script("assets/scripts/asteroids/entityDefinitions.lua")
run_script("assets/scripts/asteroids/assetDefinitions.lua")
run_script("assets/scripts/asteroids/utilities.lua")
run_script("assets/scripts/asteroids/ship.lua")
run_script("assets/scripts/asteroids/asteroid.lua")
run_script("assets/scripts/asteroids/asteroidUtilities.lua")

math.randomseed(os.time())
LoadAssets()
LoadBackground()

local ship = LoadEntity(ShipDefinitions["base_ship"])
gShip = Ship:Create({ id = ship })

main = {
    [1] = {
        Update = function()
            SpawnAsteroids()

            gShip:Update()
            UpdateAsteroids()
        end
    },
    [2] = {
        Render = function()

        end
    },
}
