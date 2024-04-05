#include "gui.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#define TEXTBOX_BUFFER_LENGTH 2048

void FBGuiLabel::draw()
{
    if (enabled) GuiEnable();
    else GuiDisable();
    GuiSetStyle(GuiControl::LABEL, GuiControlProperty::TEXT_ALIGNMENT, horizontal_align);
    GuiSetStyle(GuiControl::DEFAULT, GuiDefaultProperty::TEXT_ALIGNMENT_VERTICAL, vertical_align);

    GuiLabel(rect, ("#" + to_string(icon) + "#" + text).c_str());
}

bool FBGuiButton::wasPressed()
{
    return pressed;
}

void FBGuiButton::draw()
{
    if (enabled) GuiEnable();
    else GuiDisable();
    GuiSetStyle(GuiControl::LABEL, GuiControlProperty::TEXT_ALIGNMENT, horizontal_align);
    GuiSetStyle(GuiControl::DEFAULT, GuiDefaultProperty::TEXT_ALIGNMENT_VERTICAL, vertical_align);

    pressed = GuiButton(rect, ("#" + to_string(icon) + "#" + text).c_str());
}

bool FBGuiTextBox::wasFocusLost()
{
    return focus_lost;
}

void FBGuiTextBox::setText(const char* str)
{
    memset(buffer, '\0', TEXTBOX_BUFFER_LENGTH);
    memcpy(buffer, str, strlen(str));
}

string FBGuiTextBox::getText()
{
    return string(buffer);
}

void FBGuiTextBox::draw()
{
    if (enabled) GuiEnable();
    else GuiDisable();
    GuiSetStyle(GuiControl::LABEL, GuiControlProperty::TEXT_ALIGNMENT, horizontal_align);
    GuiSetStyle(GuiControl::DEFAULT, GuiDefaultProperty::TEXT_ALIGNMENT_VERTICAL, vertical_align);

    focus_lost = false;
    if (GuiTextBox(rect, buffer, TEXTBOX_BUFFER_LENGTH, focussed))
    {
        focussed = !focussed;
        if (!focussed)
        {
            focus_lost = true;
        }
    }
}

FBGuiTextBox::FBGuiTextBox()
{
    buffer = new char[TEXTBOX_BUFFER_LENGTH];
    memset(buffer, '\0', TEXTBOX_BUFFER_LENGTH);
}

FBGuiTextBox::~FBGuiTextBox()
{
    delete buffer;
}

int FBGuiListView::getState()
{
    return result;
}

void FBGuiListView::draw()
{
    if (enabled) GuiEnable();
    else GuiDisable();
    GuiSetStyle(GuiControl::LABEL, GuiControlProperty::TEXT_ALIGNMENT, GuiTextAlignment::TEXT_ALIGN_CENTER);
    GuiSetStyle(GuiControl::DEFAULT, GuiDefaultProperty::TEXT_ALIGNMENT_VERTICAL, GuiTextAlignmentVertical::TEXT_ALIGN_MIDDLE);

    result = GuiListView(rect, options.c_str(), &scroll_index, &active_index) ? active_index : -1;
}

int FBGuiMessageModal::getState()
{
    return result;
}

void FBGuiMessageModal::draw()
{
    if (enabled) GuiEnable();
    else GuiDisable();

    result = GuiMessageBox(rect, title.c_str(), message.c_str(), options.c_str());
}

string FBGuiTextModal::getText()
{
    return string(buffer);
}

void FBGuiTextModal::draw()
{
    if (enabled) GuiEnable();
    else GuiDisable();

    result = GuiTextInputBox(rect, title.c_str(), message.c_str(), options.c_str(), buffer, TEXTBOX_BUFFER_LENGTH, &secret_view_active);
}

FBGuiTextModal::FBGuiTextModal()
{
    buffer = new char[TEXTBOX_BUFFER_LENGTH];
    memset(buffer, '\0', TEXTBOX_BUFFER_LENGTH);
}

FBGuiTextModal::~FBGuiTextModal()
{
    delete buffer;
}

void FBGuiPanel::draw()
{
    if (enabled) GuiEnable();
    else GuiDisable();

    GuiDrawRectangle(rect, border_width, border, background);
}
