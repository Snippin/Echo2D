# Circle Collider

## Description

2D circle used for collision detection. It positions itself based on the
entity's `Transform` position.

## Properties

### `Radius (float)`

Radius of the collider.

### `Offset (Vec2)`

An offset of to origin of the game object. Origin is based on
`Transform`'s position.

### `IsColliding (bool)`

Flag to inform if the collider is touching another collider.

## Constructors

### `CircleCollider(float)`

- brief: Creates a new circle collider component
- params: Radius
- return: Newly created `CircleCollider`

```lua
local collider = entity:AddComponent(
    CicleCollider(16)
)
```

### `CircleCollider(float, Vec2)`

- brief: Creates a new circle collider component
- params: Radius, offset
- return: Newly created `CircleCollider`

```lua
local collider = entity:AddComponent(
    CicleCollider(16, Vec2(-5, 0))
)
```
