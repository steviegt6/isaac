local mod = RegisterMod("rustic", 1)
local font = Font()
font:Load("font/terminus.fnt")

--RUST = false

local function onRender()
    if RUST then
        font:DrawString("rust loaded!", 100, 100, KColor(1, 1, 1, 1), 0, true)
    else
        font:DrawString("rust not loaded!", 100, 100, KColor(1, 1, 1, 1), 0, true)
    end
end
mod:AddCallback(ModCallbacks.MC_POST_RENDER, onRender)
