CollisionSystem = {}
CollisionSystem.__index = CollisionSystem

function CollisionSystem:Create()
    local this = {
        -- TODO : Add variables as required
    }

    setmetatable(this, self)
    return this
end

function CollisionSystem:Update()
    self:UpdateCircleCollision()
end

function CollisionSystem:UpdateCircleCollision()
    local entities = Registry.GetEntities(CircleCollider)
    local to_destroy = {}

    entities:ForEach(
        function(a)
            local name_a = a:Name()
            local group_a = a:Group()
            local collider_a = a:GetComponent(CircleCollider)

            entities:ForEach(
                function(b)
                    if a:ID() == b:ID() then
                        goto continue
                    end

                    local group_b = b:Group()

                    if group_a == group_b then
                        goto continue
                    end

                    local name_b = b:Name()
                    local collider_b = b:GetComponent(CircleCollider)

                    if collider_a.IsColliding or collider_b.IsColliding then
                        goto continue
                    end

                    if self:Intersect(a, b) then
                        -- Check if projectile hit asteroids
                        if (group_a == "Projectile" and group_b == "Asteroid") or
                            (group_b == "Projectile" and group_a == "Asteroid") then
                            collider_a.IsColliding = true
                            collider_b.IsColliding = true

                            if group_a == "Asteroid" then
                                table.insert(to_destroy, a:ID())
                            else
                                table.insert(to_destroy, b:ID())
                            end
                        elseif (name_a == "Ship" and group_b == "Asteroid") or
                            (group_a == "Asteroid" and name_b == "Ship") then
                            if name_a == "Ship" then
                                collider_a.IsColliding = true
                                table.insert(to_destroy, a:ID())
                            else
                                collider_b.IsColliding = true
                                table.insert(to_destroy, b:ID())
                            end
                        end
                    end

                    ::continue::
                end
            )
        end
    )

    for key, value in pairs(to_destroy) do
        local entity = Entity(value)
        if entity:Group() == "Asteroid" then
            RemoveAsteroid(entity:ID())
        elseif entity:Name() == "Ship" then
            GAME_DATA:RemoveLife()
        end
    end
end

function CollisionSystem:Intersect(a, b)
    local a_centre = a:GetComponent(Transform).Position
    local b_centre = b:GetComponent(Transform).Position

    -- Get vector difference
    local difference = a_centre - b_centre

    -- Get distance squared
    local distance_sqr = difference:LengthSqr()

    -- Get radius of circle collider
    local circle_a = a:GetComponent(CircleCollider)
    local circle_b = b:GetComponent(CircleCollider)

    local radius_sum = circle_a.Radius + circle_b.Radius
    local radius_sqr = radius_sum * radius_sum

    return distance_sqr <= radius_sqr
end
