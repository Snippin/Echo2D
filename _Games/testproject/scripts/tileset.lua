Tileset = {}
Tileset.__index = Tileset

function Tileset:Create(params)
    local this = {
        Name = params.name,
        Columns = params.columns,
        Width = params.width,
        Height = params.height,
        TileWidth = params.tilewidth,
        TileHeight = params.tileheight,
        FirstGId = params.firstgid,
    }

    this.Rows = math.floor(params.height / params.tileheight)
    this.LastGId = (this.Rows * this.Columns + this.FirstGId) - 1

    setmetatable(this, self)
    return this
end

function Tileset:DoesIdExist(id)
    return id >= self.FirstGId and id <= self.LastGId
end

function Tileset:GetTileStartPos(id)
    assert(self:DoesIdExist(id), string.format(
        "Tile ID [%i] does not exist in tileset [%s]", id, self.Name))

    local actual_tile_id = id - self.FirstGId
    local y = math.floor(actual_tile_id / self.Columns)
    local x = math.floor(actual_tile_id % self.Columns)

    return x, y
end
