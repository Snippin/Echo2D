-- Main lua script
run_script("../_Games/asteroids/scripts/main.lua")

main = {
    [1] = {
        Update = function()
            AsteroidsGame()
        end
    },
    [2] = {
        Render = function()

        end
    },
}
