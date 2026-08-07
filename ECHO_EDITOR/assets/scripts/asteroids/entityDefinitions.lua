AsteroidDefinitions =
{
    big_asteroid = {
        group = "Asteroid",
        components = {
            transform = {
                position = { x = -200, y = -100 },
                scale = { x = 1, y = 1 },
                rotation = 0
            },
            sprite = {
                assetName = "asteroid_big",
                width = 136,
                height = 111,
                start_x = 0,
                start_y = 0,
                layer = 2
            }
        },
        type = "Big",
        min_speed = -2,
        max_speed = 5,
    },
    small_asteroid = {
        group = "Asteroid",
        components = {
            transform = {
                position = { x = 200, y = -100 },
                scale = { x = 1, y = 1 },
                rotation = 0
            },
            sprite = {
                assetName = "asteroid_small",
                width = 44,
                height = 42,
                start_x = 0,
                start_y = 0,
                layer = 2
            }
        },
        type = "Small",
        min_speed = -5,
        max_speed = 5,
    }
}

ShipDefinitions = {
    base_ship = {
        name = "Ship",
        components = {
            transform = {
                position = { x = 0, y = 0 },
                scale = { x = 1, y = 1 },
                rotation = 0
            },
            sprite = {
                assetName = "ship",
                width = 99,
                height = 75,
                start_x = 0,
                start_y = 0,
                layer = 2,
                rotation = 90
            }
        }
    }
}
