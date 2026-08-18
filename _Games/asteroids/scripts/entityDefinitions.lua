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
            },
            circle_collider = {
                radius = 55
            },
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
            },
            circle_collider = {
                radius = 21
            },
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
            },
            circle_collider = {
                radius = 37.5
            },
            animation = {
                frames = 0,
                frame_rate = 0,
                frame_offset = 0,
                is_vertical = false,
                is_looped = false,

            },
        }
    }
}

ProjectileDefinitions = {
    projectile1 = {
        group = "Projectile",
        components = {
            transform = {
                position = { x = 0, y = 0 },
                scale = { x = 1, y = 1 },
                rotation = 0
            },
            sprite = {
                assetName = "projectile_1",
                width = 9,
                height = 33,
                start_x = 0,
                start_y = 0,
                layer = 2,
                rotation = 90
            },
            circle_collider = {
                radius = 16
            },
        },
        lifetime = 2000,
        speed = 10,
    }
}

HudDefinitions = {
    lives = {
        group = "Life",
        components = {
            transform = {
                position = { x = 0, y = 0 },
                scale = { x = 1, y = 1 },
                rotation = 0
            },
            sprite = {
                assetName = "life",
                width = 35,
                height = 27,
                start_x = 0,
                start_y = 0,
                layer = 10,
            },
        },
    },
    number = {
        Group = "Number",
        components = {
            transform = {
                position = { x = 0, y = 0 },
                scale = { x = 1, y = 1 },
                rotation = 0
            },
            sprite = {
                assetName = "numbers_text",
                width = 19,
                height = 19,
                start_x = 0,
                start_y = 0,
                layer = 10,
            },
        },
    },
    game_over = {
        name = "GameOver",
        components = {
            transform = {
                position = { x = 0, y = 0 },
                scale = { x = 1, y = 1 },
                rotation = 0
            },
            sprite = {
                assetName = "game_over_text",
                width = 282,
                height = 61,
                start_x = 0,
                start_y = 0,
                layer = 10,
                is_hidden = true,
            },
        },
    },
}
