-- Helper functions

function LoadEntity(def)
    assert(def, "Def does not exist")

    local name = ""
    if def.name then
        name = def.name
    end

    local group = ""
    if def.group then
        group = def.group
    end

    local new_entity = Entity(name, group)

    if def.components.transform then
        new_entity:AddComponent(
            Transform(
                Vec2(
                    def.components.transform.position.x,
                    def.components.transform.position.y
                ),
                Vec2(
                    def.components.transform.scale.x,
                    def.components.transform.scale.y
                ),
                def.components.transform.rotation
            )
        )
    end

    if def.components.sprite then
        local sprite = new_entity:AddComponent(
            Sprite(
                def.components.sprite.assetName,
                def.components.sprite.width,
                def.components.sprite.height,
                def.components.sprite.start_x,
                def.components.sprite.start_y,
                def.components.sprite.layer
            )
        )
        sprite:GenerateUVs()
    end

    return new_entity:ID()
end
