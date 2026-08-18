Hud = {}
Hud.__index = Hud

function Hud:Create()
    local this = {
        Lives = {},
        CurrentLives = GAME_DATA:GetMaxLives(),
        Scores = {},
        Score = 0,
    }

    -- Create life entities (images)
    for i = 1, GAME_DATA:GetMaxLives() - 1 do
        local lives_id = LoadEntity(HudDefinitions["lives"])
        local lives_entity = Entity(lives_id)
        local transform = lives_entity:GetComponent(Transform)
        transform.Position = Vec2(-WINDOW_HALF.x + 40 * i, -WINDOW_HALF.y + 27)
        table.insert(this.Lives, lives_id)
    end

    -- Score entities (max score = 99999)
    local sprite_width = 19

    for i = 0, 4 do
        local score_id = LoadEntity(HudDefinitions["number"])
        local score_entity = Entity(score_id)
        local transform = score_entity:GetComponent(Transform)
        transform.Position = Vec2(
            WINDOW_HALF.x - sprite_width * (5 - i),
            -WINDOW_HALF.y + sprite_width)
        table.insert(this.Scores, score_id)
    end

    -- Game over entity
    this.IsGameOver = false
    this.ShowGameOver = false
    this.GameOverID = LoadEntity(HudDefinitions["game_over"])

    setmetatable(this, self)
    return this
end

function Hud:Update()
    if not self.IsGameOver then
        self:UpdateLives()
        self:UpdateScore()
    elseif not self.ShowGameOver then
        local entity = Entity(self.GameOverID)
        local sprite = entity:GetComponent(Sprite)
        sprite.IsHidden = false
        self.ShowGameOver = true
    end
end

function Hud:UpdateLives()
    local num_lives = GAME_DATA:GetNumLives()

    if self.CurrentLives ~= num_lives then
        for key, value in pairs(self.Lives) do
            local sprite = Entity(value):GetComponent(Sprite)
            if key <= num_lives then
                sprite.IsHidden = false
            else
                sprite.IsHidden = true
            end
        end

        if num_lives == 0 then
            self.IsGameOver = true
        end

        self.CurrentLives = num_lives
    end
end

function Hud:UpdateScore()
    local current_score = GAME_DATA:GetScore()

    if self.Score == current_score then
        return
    end

    self:SetScore(current_score)
end

function Hud:SetScore(score)
    -- Get digit values
    local digits = {
        GetDigitValue(score, 5),
        GetDigitValue(score, 4),
        GetDigitValue(score, 3),
        GetDigitValue(score, 2),
        GetDigitValue(score, 1),
    }

    -- Set sprite uvs
    for i = 1, 5 do
        local sprite = Entity(self.Scores[i]):GetComponent(Sprite)
        sprite.Start_X = digits[i]
        sprite.UVs.X = sprite.UVs.UV_Width * sprite.Start_X
    end

    -- Set score
    self.Score = score
end

function Hud:Reset()
    self.IsGameOver = false
    self.ShowGameOver = false
    self.Score = 0

    local sprite = Entity(self.GameOverID):GetComponent(Sprite)
    sprite.IsHidden = true
end
