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
