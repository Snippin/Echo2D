AssetDefinitions = {
    textures = {
        {
            name = "ship",
            path = "../_Games/asteroids/textures/ship.png",
            pixel_art = true
        },
        {
            name = "asteroid_small",
            path = "../_Games/asteroids/textures/asteroid_small.png",
            pixel_art = true
        },
        {
            name = "asteroid_big",
            path = "../_Games/asteroids/textures/asteroid_big.png",
            pixel_art = true
        },
        {
            name = "star_bg",
            path = "../_Games/asteroids/textures/star_bg.png",
            pixel_art = true
        },
        {
            name = "projectile_1",
            path = "../_Games/asteroids/textures/projectile_1.png",
            pixel_art = true
        },
        {
            name = "life",
            path = "../_Games/asteroids/textures/life.png",
            pixel_art = true
        },
        {
            name = "numbers_text",
            path = "../_Games/asteroids/textures/numbers_text.png",
            pixel_art = true
        },
        {
            name = "game_over_text",
            path = "../_Games/asteroids/textures/game_over_text.png",
            pixel_art = true
        },
        {
            name = "explosion",
            path = "../_Games/asteroids/textures/explosion.png",
            pixel_art = true
        },
    },
    musics = {
        {
            name = "gameplay_music",
            path = "../_Games/asteroids/audio/gameplay_music.wav",
        },
    },
    sound_fx = {
        {
            name = "sfx_laser",
            path = "../_Games/asteroids/audio/sfx_laser.ogg",
        },
        {
            name = "sfx_big_asteroid_explosion",
            path = "../_Games/asteroids/audio/sfx_big_asteroid_explosion.wav",
        },
        {
            name = "sfx_small_asteroid_explosion",
            path = "../_Games/asteroids/audio/sfx_small_asteroid_explosion.ogg",
        },
        {
            name = "sfx_ship_explosion",
            path = "../_Games/asteroids/audio/sfx_ship_explosion.wav",
        },
    },
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

    for key, value in pairs(AssetDefinitions.musics) do
        if not AssetManager.AddMusic(value.name, value.path) then
            print(string.format("Failed to load music [%s] at path [%s]",
                value.name, value.path))
        else
            print(string.format("Loaded music [%s]", value.name))
        end
    end

    for key, value in pairs(AssetDefinitions.sound_fx) do
        if not AssetManager.AddSoundFX(value.name, value.path) then
            print(string.format("Failed to load soundfx [%s] at path [%s]",
                value.name, value.path))
        else
            print(string.format("Loaded soundfx [%s]", value.name))
        end
    end
end
