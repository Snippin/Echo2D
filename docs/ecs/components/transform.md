# Transform

## Description

Contains **Position**, **Scale**, and, **Rotation** of an entity.

## Properties

### `Position (Vec2)`

World coordinates of the entity

### `Scale (Vec2)`

Scale of the entity

### `Rotation (float)`

Rotation of the entity in degrees

## Constructors

### `Transform(Vec2, Vec2, float)`

- brief: Creates a new transform component
- params: Position, scale, rotation
- return: Newly created `Transform`

```lua
local transform = entity:AddComponent(
    Transform(Vec2(0), Vec2(20), 0)
)
```

### `Transform(float, float, float, float, float)`

- brief: Creates a new transform component
- params: X position, y position, x scale, y scale, rotation
- return: Newly created `Transform`

```lua
local transform = entity:AddComponent(
    Transform(0, 0, 20, 20, 0)
)
```
