#pragma once

#include "raygui.h"

#define STYLE_PROPS_COUNT 10

static const GuiStyleProp styleProperties[STYLE_PROPS_COUNT] = {
    { GuiControl::DEFAULT, GuiControlProperty::BORDER_COLOR_NORMAL, 0xab9bd3ff },
    { GuiControl::DEFAULT, GuiControlProperty::BASE_COLOR_NORMAL, 0x3e4350ff },
    { GuiControl::DEFAULT, GuiControlProperty::TEXT_COLOR_NORMAL, 0xdadaf4ff },
    { GuiControl::DEFAULT, GuiControlProperty::BORDER_COLOR_DISABLED, 0x8fa2bdff },
    { GuiControl::DEFAULT, GuiControlProperty::BASE_COLOR_DISABLED, 0x6b798dff },
    { GuiControl::DEFAULT, GuiControlProperty::TEXT_COLOR_DISABLED, 0x8292a9ff },
    { GuiControl::DEFAULT, GuiDefaultProperty::TEXT_SIZE, 0x00000010 },
    { GuiControl::DEFAULT, GuiDefaultProperty::LINE_COLOR, 0x84adb7ff },
    { GuiControl::DEFAULT, GuiDefaultProperty::BACKGROUND_COLOR, 0x5b5b81ff },
    { GuiControl::DEFAULT, GuiDefaultProperty::TEXT_SIZE, 20 }
};