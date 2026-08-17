SMALL_ASTEROID_SCORE = 15
BIG_ASTEROID_SCORE = 100

GameData = {}
GameData.__index = GameData

function GameData:Create()
    local this = {
        MaxLives = 5,
        NumLives = 3,
        NumAsteroids = 0,
        Score = 0,
    }

    setmetatable(this, self)
    return this
end

function GameData:IsGameOver()
    return self.NumLives <= 0
end

function GameData:GetMaxLives()
    return self.MaxLives
end

function GameData:GetNumLives()
    return self.NumLives
end

function GameData:AddLife()
    self.NumLives = Clamp(self.NumLives + 1, 0, self.MaxLives)
end

function GameData:RemoveLife()
    self.NumLives = Clamp(self.NumLives - 1, 0, self.MaxLives)
end

function GameData:AddScore(value)
    self.Score = Clamp(self.Score + value, 0, 99999)
end

function GameData:GetScore()
    return self.Score
end

function GameData:Reset()
    self.NumLives = 3
    self.NumAsteroids = 0
    self.Score = 0
end

GAME_DATA = GameData:Create()
