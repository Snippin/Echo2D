# Sprite

## Description

Contains all necessary properties for the sprite of an entity.

## Properties

### `Width (float)`

Width of the sprite in pixels.

### `Height (float)`

Height of the sprite in pixels.

### `Color (Color)`

Color of the sprite. Defaults to white (255, 255, 255, 255).

### `StartX (int)`

Start x position for use in a texture atlas.

### `StartY (int)`

Start y position for use in a texture atlas.

### `Layer (int)`

Layer determines when the sprite gets rendered. Lower layers get rendered first.

### `Rotation (float)`

Rotation offset in case of texture not facing the correct orientation when
`Transform`'s rotation is 0.

### `IsHidden (bool)`

Flag to determine whether the sprite should be rendered.

### `TextureName (string)`

Name of the sprite to be used. The name **MUST** already exist in `AssetManager`
before usage.

## Functions

### `GenerateUVs()`

**MUST** be called after adding sprite component to generate the uvs. Else,
sprite will not render

- brief: Populates the UV values for the texture atlas
- params: -
- return: -

```lua
local sprite = entity:AddComponent(
    Sprite("ship", 16, 16, 0, 0, 2)
)
sprite:GenerateUVs()
```

## Constructors

### `Sprite(string, float, float, int, int, int)`

- brief: Creates a new sprite component
- params: Texture name, sprite pixel width, sprite pixel height, atlas start x
  position, atlas start y position, layer
- return: Newly created `Sprite`

```lua
local sprite = entity:AddComponent(
    Sprite("ship", 16, 16, 0, 0, 0)
)
```

### `Sprite(string, float, float, int, int , int, float)`

- brief: Creates a new sprite component
- params: Texture name, sprite pixel width, sprite pixel height, atlas start x
  position for use in texture atlas, atlas start y position, layer, sprite
  rotation offset
- return: Newly created `Sprite`

```lua
local sprite = entity:AddComponent(
    Sprite("ship", 16, 16, 0, 0, 0, 90)
)
```
