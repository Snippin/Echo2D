run_script("../_Games/asteroids/scripts/utilities.lua")
run_script("../_Games/asteroids/scripts/entityDefinitions.lua")
run_script("../_Games/asteroids/scripts/assetDefinitions.lua")

run_script("../_Games/asteroids/scripts/ship.lua")
run_script("../_Games/asteroids/scripts/asteroid.lua")
run_script("../_Games/asteroids/scripts/asteroidUtilities.lua")
run_script("../_Games/asteroids/scripts/collisionSystem.lua")
run_script("../_Games/asteroids/scripts/projectile.lua")
run_script("../_Games/asteroids/scripts/projectileUtilities.lua")

run_script("../_Games/asteroids/scripts/gameData.lua")
run_script("../_Games/asteroids/scripts/hud.lua")

math.randomseed(os.time())
LoadAssets()
LoadBackground()

local ship_id = LoadEntity(ShipDefinitions["base_ship"])

local ship = Ship:Create({ id = ship_id })
local collision_system = CollisionSystem:Create()
local hud = Hud:Create()

ship:Reset()
Music.Play("gameplay_music")

function AsteroidsGame()
    if Keyboard.JustPressed(KEY_Z) then
        Music.Stop()
    end

    if not GAME_DATA:IsGameOver() then
        SpawnAsteroids()
    else
        if Keyboard.JustPressed(KEY_ENTER) then
            GAME_DATA:Reset()
            hud:Reset()
            ship:Reset()
            ResetAsteroids()
            ResetProjectiles()
        end
    end

    ship:Update()
    UpdateAsteroids()
    UpdateProjectiles()
    collision_system:Update()
    hud:Update()
end
