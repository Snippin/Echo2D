# `RuntimeView`

## Description

A view over entities matching a set of component requirements upon calling
`Registry:GetEntities`.

## Functions

### `ForEach(function(entity))`

- brief: Iterates over all of the entities that are in `runtime_view`
- params: Lambda function that takes in the current entity
- return: -

```lua
local view = Registry.GetEntities(Transform, Sprite, ...)
view:ForEach(
    function(entity)
        ...
    end
)
```

### `Exclude(variadic_args)`

- brief: Removes desired entities from `runtime_view`
- params: Components in entities have that is being removed
- return: -

```lua
local view = Registry.GetEntities(Transform, Sprite, ...)
view:Exclude(CircleCollider, ...)
```

# `Registry`

## Description

`Registry` holds references to all different entities that are added to it and
is able to query them and their components.

## Functions

### `GetEntities(variadic_args)`

- brief: Gets all entities with specified components
- params: Components that the entity has
- return: `RuntimeView` which is a list of entities with the specified
  components

```lua
local view = Registry.GetEntities(Transform, Sprite, ...)
```

### `Clear()`

- brief: Clears all registered entities in `Registry`
- params: -
- return: -

```lua
local view = Registry.Clear()
```
