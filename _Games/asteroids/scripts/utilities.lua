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
                def.components.sprite.layer,
                def.components.sprite.rotation or 0
            )
        )
        sprite:GenerateUVs()
        sprite.IsHidden = def.components.sprite.is_hidden or false
    end

    if def.components.circle_collider then
        local collider = new_entity:AddComponent(
            CircleCollider(def.components.circle_collider.radius)
        )
    end

    if def.components.animation then
        local animation = new_entity:AddComponent(
            Animation(
                def.components.animation.frames,
                def.components.animation.frame_rate,
                def.components.animation.frame_offset,
                def.components.animation.is_vertical,
                def.components.animation.is_looped
            )
        )
    end

    return new_entity:ID()
end

function LoadBackground()
    for i = 0, 2 do
        for j = 0, 3 do
            local bg_tile = Entity("", "Background")
            bg_tile:AddComponent(
                Transform(
                    Vec2(j * 256 - WINDOW_HALF.x, i * 256 - WINDOW_HALF.y),
                    Vec2(1, 1), 0
                )
            )
            local sprite =
                bg_tile:AddComponent(Sprite("star_bg", 256, 256, 0, 0, 0))
            sprite:GenerateUVs()
        end
    end
end

-- position = Vec2, scale = Vec2, size = Vec2
function CheckPosition(position, scale, size)
    local min_x = -WINDOW_HALF.x
    local min_y = -WINDOW_HALF.y
    local max_x = WINDOW_HALF.x
    local max_y = WINDOW_HALF.y

    local width = size.x * 0.5 * scale.x
    local height = size.y * 0.5 * scale.y

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

function GetRandomVelocity(min_speed, max_speed)
    return Vec2(
        math.random(min_speed, max_speed),
        math.random(min_speed, max_speed)
    )
end

function GetRandomPosition()
    return Vec2(
        math.random(-WINDOW_HALF.x, WINDOW_HALF.x) + WINDOW_WIDTH,
        math.random(-WINDOW_HALF.y, WINDOW_HALF.y) + WINDOW_HEIGHT
    )
end

function GetDigitValue(num, digit)
    local n = 10 ^ digit
    local n1 = 10 ^ (digit - 1)
    return math.floor((num % n) / n1)
end
