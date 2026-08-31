# Box Collider

## Description

Axis-aligned rectangle used for collision detection. It positions itself based
on the entity's `Transform` position.

## Properties

### `Width (int)`

Width of the collider.

### `Height (int)`

Height of the collider.

### `Offset (Vec2)`

An offset of to origin of the game object. Origin is based on
`Transform`'s position.

### `IsColliding (bool)`

Flag to inform if the collider is touching another collider.

## Constructors

### `BoxCollider(int, int)`

- brief: Creates a new box collider component
- params: Width, height
- return: Newly created `BoxCollider`

```lua
local collider = entity:AddComponent(
    BoxCollider(5, 5)
)
```

### `BoxCollider(int, int, Vec2)`

- brief: Creates a new box collider component
- params: Width, height, offset
- return: Newly created `BoxCollider`

```lua
local collider = entity:AddComponent(
    BoxCollider(5, 5, Vec2(-5, 0))
)
```
