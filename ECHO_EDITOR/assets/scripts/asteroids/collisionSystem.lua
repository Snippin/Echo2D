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

    entities:ForEach(
        function(a)
            local group_a = a:Group()
            local collider_a = a:GetComponent(CircleCollider)

            entities:ForEach(
                function(b)
                    if a:ID() == b:ID() then
                        goto continue
                    end

                    local group_b = b:Group()

                    if group_a == group_b then
                        printf("ASteroid")
                        goto continue
                    end

                    local collider_b = b:GetComponent(CircleCollider)

                    if collider_a.IsColliding or collider_b.IsColliding then
                        goto continue
                    end

                    if self:Intersect(a, b) then
                        print(string.format("ID: [%s] is colliding with ID: [%s]",
                            a:ID(), b:ID()))
                        -- TODO : Add destruction of asteroids
                    end

                    ::continue::
                end
            )
        end
    )
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
