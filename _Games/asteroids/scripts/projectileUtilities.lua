local projectiles = {}

function AddPojectile(projectile)
    table.insert(projectiles, projectile)
end

function UpdateProjectiles()
    for key, value in pairs(projectiles) do
        if value:IsLifetimeOver() then
            value:Destroy()
            projectiles[key] = nil
        else
            value:Update()
        end
    end
end

function ResetProjectiles()
    for key, value in pairs(projectiles) do
        local projectile = Entity(value.ID)
        projectile:Kill()
        projectiles[key] = nil
    end
end
