AssetDefinitions = {
    textures = {
        {
            name = "ship",
            path = "assets/textures/asteroids/ship.png",
            pixel_art = true
        },
        {
            name = "asteroid_small",
            path = "assets/textures/asteroids/asteroid_small.png",
            pixel_art = true
        },
        {
            name = "asteroid_big",
            path = "assets/textures/asteroids/asteroid_big.png",
            pixel_art = true
        },
        {
            name = "star_bg",
            path = "assets/textures/asteroids/star_bg.png",
            pixel_art = true
        },
        {
            name = "projectile_1",
            path = "assets/textures/asteroids/projectile_1.png",
            pixel_art = true
        },
    }
}

function LoadAssets()
    for key, value in pairs(AssetDefinitions.textures) do
        if not AssetManager.AddTexture(value.name, value.path, value.pixel_art)
        then
            print(string.format("Failed to load texture [%s] at path [%s]",
                value.name, value.path))
        else
            print(string.format("Loaded texture [%s]", value.name))
        end
    end

    -- TODO: Load other asset types
end
