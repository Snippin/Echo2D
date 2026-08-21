run_script("../_Games/testproject/scripts/assetdef.lua")
run_script("../_Games/testproject/scripts/map/testmap.lua")

run_script("../_Games/testproject/scripts/utilities.lua")
run_script("../_Games/testproject/scripts/tileset.lua")

LoadAssets(AssetDefinitions)

local tilemap = CreateTestMap()
LoadMap(tilemap)

function UpdateTestProject()

end
