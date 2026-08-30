# AssetManager

## Description

`AssetManager` stores textures, fonts, music, soundfx, and shaders.

Assets are required to be added to it first before usage.

## Functions

### `AddTexture(string, string, bool)`

- brief: Attempt to add a new texture to `AssetManager`
- params: Asset name, texture file path, flag if texture is pixel art
- return: True if successful, false otherwise

```lua
if AssetManager.AddTexture("hero", "./assets/textures/hero.png", true) then
    print("Added texture successfully")
else
    print("Failed to add texture")
end
```

### `AddMusic(string, string)`

- brief: Attempt to add new music to `AssetManager`
- params: Asset name, texture file path
- return: True if successful, false otherwise

```lua
if AssetManager.AddMusic("title_music", "./assets/audio/title_music.wav") then
    print("Added music successfully")
else
    print("Failed to add music")
end
```

### `AddSoundFX(string, string)`

- brief: Attempt to add a new soundfx to `AssetManager`
- params: Asset name, texture file path
- return: True if successful, false otherwise

```lua
if AssetManager.AddSoundFX("sword_sfx", "./assets/audio/sword_sfx.wav") then
    print("Added soundfx successfully")
else
    print("Failed to add soundfx")
end
```
