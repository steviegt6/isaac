--[[
    This file is part of Testament, a part of the Testament Project hosted
    within the mono-repository located at <https://github.com/steviegt6/isaac>.
    
    Testament is released under the GNU General Public License, version 3. As
    such, it is subject to the terms written in the `LICENSE-GPL` file located
    within the project root.
]]

local mod = RegisterMod("Testament", 1)
local game = Game()

local font = Font()
font:Load("font/terminus.fnt")

local skippedMessage = false

local function shouldShowAnnoyingMessage()
    return not testament and not skippedMessage
end

local function postUpdate()
    if not game:IsPaused() and Input.IsActionTriggered(ButtonAction.ACTION_MENUCONFIRM, Isaac.GetPlayer(0).ControllerIndex) then
        skippedMessage = true
    end
end
mod:AddCallback(ModCallbacks.MC_POST_UPDATE, postUpdate)

local function postRender()
    if shouldShowAnnoyingMessage() then
        local messages = {
            "Testament is enabled but is not installed.",
            "Go to:",
            "https://github.com/steviegt6/isaac#readme",
            "to learn how to properly install Testament.",
            "",
            "---> Not doing so WILL BREAK YOUR MODS!!!!!!! <---",
            "",
            "If this message still shows up, you did not install",
            "Testament correctly.",
            "",
            "Ask for help at:",
            "https://discord.gg/Y8bvvqyFQw",
            "",
            "To disregard this warning, press ENTER/SPACE.",
            "(A on a controller)"
        }

        local white = KColor(1, 1, 1, 1)
        local red = KColor(1, 0, 0, 1)
        local blue = KColor(0.2, 0.2, 1, 1)

        local colors = {
            white,
            white,
            blue,
            white,
            white,
            red,
            white,
            white,
            white,
            white,
            white,
            blue,
            white,
            white,
            white
        }

        for i in pairs(messages) do
            font:DrawString(messages[i], 60, (i * 15) + 15, colors[i], 0, 0, false)
        end
    end
end
mod:AddCallback(ModCallbacks.MC_POST_RENDER, postRender)