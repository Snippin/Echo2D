-- Helper functions

WINDOW_WIDTH = 640
WINDOW_HEIGHT = 480
WINDOW_HALF = { x = WINDOW_WIDTH * 0.5, y = WINDOW_HEIGHT * 0.5 }

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

-- position = Vec2, width = float, height = float
function CheckPosition(position, width, height)
    local min_x = -WINDOW_HALF.x
    local min_y = -WINDOW_HALF.y
    local max_x = WINDOW_HALF.x
    local max_y = WINDOW_HALF.y

    if position.x + width < min_x then
        position.x = position.x + WINDOW_WIDTH + width
    elseif position.x > max_x + width then
        position.x = position.x - WINDOW_WIDTH - width
    end

    if position.y + height < min_y then
        position.y = position.y + WINDOW_HEIGHT + height
    elseif position.y > max_y + height then
        position.y = position.y - WINDOW_HEIGHT - height
    end
end
