WINDOW_WIDTH = 640
WINDOW_HEIGHT = 480
WINDOW_HALF = { x = WINDOW_WIDTH * 0.5, y = WINDOW_HEIGHT * 0.5 }

function LoadAssets(assets)
    for key, value in pairs(assets.textures) do
        if not AssetManager.AddTexture(value.name, value.path, value.pixel_art)
        then
            print(string.format("Failed to load texture [%s] at path [%s]",
                value.name, value.path))
        else
            print(string.format("Loaded texture [%s]", value.name))
        end
    end

    for key, value in pairs(assets.musics) do
        if not AssetManager.AddMusic(value.name, value.path) then
            print(string.format("Failed to load music [%s] at path [%s]",
                value.name, value.path))
        else
            print(string.format("Loaded music [%s]", value.name))
        end
    end

    for key, value in pairs(assets.sound_fx) do
        if not AssetManager.AddSoundFX(value.name, value.path) then
            print(string.format("Failed to load soundfx [%s] at path [%s]",
                value.name, value.path))
        else
            print(string.format("Loaded soundfx [%s]", value.name))
        end
    end
end

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

function LoadMap(map_def)
    assert(map_def)

    local tilesets = {}
    for k, v in pairs(map_def.tilesets) do
        local new_tileset = Tileset:Create(
            {
                name = v.name,
                columns = v.columns,
                width = v.imagewidth,
                height = v.imageheight,
                tilewidth = v.tilewidth,
                tileheight = v.tileheight,
                firstgid = v.firstgid,
            }
        )

        table.insert(tilesets, new_tileset)
    end

    -- Loop through layers and create tile
    for k, v in pairs(map_def.layers) do
        local rows = v.height - 1
        local columns = v.width
        local layer = k - 1

        for row = 0, rows do
            for col = 1, columns do
                local id = v.data[row * columns + col]

                if id == 0 then
                    goto continue
                end

                local tileset = GetTileset(tilesets, id)
                assert(tileset,
                    string.format("Tileset does not exist with id [%i]", id))

                local x, y = tileset:GetTileStartPos(id)
                local scale = 2
                local position = Vec2(
                    ((col - 1) * tileset.TileWidth + tileset.TileWidth * 0.5) * scale - WINDOW_HALF.x,
                    (row * tileset.TileHeight + tileset.TileHeight * 0.5) * scale - WINDOW_HALF.y
                )

                local entity = Entity("", "Tile")

                entity:AddComponent(Transform(position, Vec2(scale), 0))

                local sprite = entity:AddComponent(
                    Sprite(
                        tileset.Name,
                        tileset.TileWidth,
                        tileset.TileHeight,
                        x,
                        y,
                        layer
                    )
                )
                sprite:GenerateUVs()

                if tileset.Name == "collider" then
                    entity:AddComponent(
                        BoxCollider(
                            tileset.TileWidth,
                            tileset.TileHeight
                        )
                    )

                    sprite.IsHidden = true
                end

                ::continue::
            end
        end
    end
end

function GetTileset(tilesets, id)
    for k, v in pairs(tilesets) do
        if v:DoesIdExist(id) then
            return v
        end
    end

    return nil
end
