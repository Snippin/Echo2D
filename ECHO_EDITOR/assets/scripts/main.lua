-- Main lua script
run_script("../_Games/testproject/scripts/main.lua")

main = {
    [1] = {
        Update = function()
            UpdateTestProject()
        end
    },
    [2] = {
        Render = function()

        end
    },
}
