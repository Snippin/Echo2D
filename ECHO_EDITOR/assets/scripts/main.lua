-- Main lua script

run_script("assets/scripts/asteroids/entityDefinitions.lua")
run_script("assets/scripts/asteroids/assetDefinitions.lua")
run_script("assets/scripts/asteroids/utilities.lua")
run_script("assets/scripts/asteroids/ship.lua")
LoadAssets();

local ship = LoadEntity(ShipDefinitions["base_ship"])
local big_asteroid = LoadEntity(AsteroidDefinitions["big_asteroid"])
local small_asteroid = LoadEntity(AsteroidDefinitions["small_asteroid"])

gShip = Ship:Create({ id = ship })

main = {
    [1] = {
        Update = function()
            gShip:UpdateShip()
        end
    },
    [2] = {
        Render = function()

        end
    },
}
