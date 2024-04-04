#pragma once

#include "raygui.h"

#define STYLE_PROPS_COUNT 9

static const GuiStyleProp styleProperties[STYLE_PROPS_COUNT] = {
    { GuiControl::DEFAULT, BORDER_COLOR_NORMAL, 0xab9bd3ff },
    { GuiControl::DEFAULT, BASE_COLOR_NORMAL, 0x3e4350ff },
    { GuiControl::DEFAULT, TEXT_COLOR_NORMAL, 0xdadaf4ff },
    { GuiControl::DEFAULT, BORDER_COLOR_DISABLED, 0x8fa2bdff },
    { GuiControl::DEFAULT, BASE_COLOR_DISABLED, 0x6b798dff },
    { GuiControl::DEFAULT, TEXT_COLOR_DISABLED, 0x8292a9ff },
    { GuiControl::DEFAULT, GuiDefaultProperty::TEXT_SIZE, 0x00000010 },
    { GuiControl::DEFAULT, GuiDefaultProperty::LINE_COLOR, 0x84adb7ff },
    { GuiControl::DEFAULT, GuiDefaultProperty::BACKGROUND_COLOR, 0x5b5b81ff }
};