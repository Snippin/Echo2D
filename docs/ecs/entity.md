# Entity

## Description

`Entity` is a game object that consists of a unique id number and is created and
monitored by [`Registry`](registry.md).

Each entity can have various components.

## Functions

### `AddComponent(component)`

- brief: Add a component to an entity
- params: Component to add
- return: Component that was added

```lua
local transform = entity:AddComponent(Transform(...))
```

### `HasComponent(component)`

- brief: Check if an entity has a component
- params: Component to be checked
- return: True if component exists, false otherwise

```lua
if entity:HasComponent(Transform) then
    ...
end
```

### `GetComponent(component)`

- brief: Get a component from an entity
- params: Component to be retrieved
- return: Component that is specified

```lua
local transform = entity:GetComponent(Transform)
```

### `RemoveComponent(component)`

- brief: Remove a component from an entity
- params: Component to be removed
- return: Component that was removed

```lua
local transform = entity:RemoveComponent(Transform)
```

### `Name()`

- brief: Get the name of an entity
- params: -
- return: Name of entity

```lua
local name = entity:Name()
```

### `Group()`

- brief: Get the group of an entity
- params: -
- return: Group of entity

```lua
local group = entity:Group()
```

### `Kill()`

- brief: Destroys an entity and removes it from `Registry`
- params: -
- return: -

```lua
entity:Kill()
```

### `ID()`

- brief: Get the unique id of an entity
- params: -
- return: ID of entity

```lua
local id = entity:ID()
```

## Constructors

### `Entity(string, string)`

- brief: Creates a new entity
- params: Entity name, entity group
- return: Newly created entity

```lua
local entity = Entity("Player", "Characters")
```

### `Entity(int)`

- brief: Gets an entity
- params: Unique entity id
- return: `Entity` with the specified id

```lua
local entity = Entity("Player", "Characters")
local player = Entity(entity:ID())
```
