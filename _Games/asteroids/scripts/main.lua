run_script("../_Games/asteroids/scripts/entityDefinitions.lua")
run_script("../_Games/asteroids/scripts/assetDefinitions.lua")
run_script("../_Games/asteroids/scripts/utilities.lua")

run_script("../_Games/asteroids/scripts/ship.lua")
run_script("../_Games/asteroids/scripts/asteroid.lua")
run_script("../_Games/asteroids/scripts/asteroidUtilities.lua")
run_script("../_Games/asteroids/scripts/collisionSystem.lua")
run_script("../_Games/asteroids/scripts/projectile.lua")
run_script("../_Games/asteroids/scripts/projectileUtilities.lua")

run_script("../_Games/asteroids/scripts/gameData.lua")

math.randomseed(os.time())
LoadAssets()
LoadBackground()

local ship = LoadEntity(ShipDefinitions["base_ship"])
SHIP = Ship:Create({ id = ship })
COLLISION_SYSTEM = CollisionSystem:Create()

function AsteroidsGame()
    SpawnAsteroids()

    SHIP:Update()
    UpdateAsteroids()
    UpdateProjectiles()
    COLLISION_SYSTEM:Update()
end
