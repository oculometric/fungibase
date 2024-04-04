#include "editor.h"

#include <iostream>

#include "styling.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

void FBEditor::initWindow()
{
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    SetTargetFPS(30);
    InitWindow(window_width, window_height, "fungibase");
    
    for (int i = 0; i < STYLE_PROPS_COUNT; i++)
        GuiSetStyle(styleProperties[i].controlId, styleProperties[i].propertyId, styleProperties[i].propertyValue);

    window_width = GetMonitorWidth(0) / 2;
    window_height = GetMonitorHeight(0) / 2;
    SetWindowSize(window_width, window_height);
    SetWindowPosition(window_width / 2, window_height / 2);

    while (!WindowShouldClose())
    {
        if (IsWindowResized())
        {
            updateFrameSizes(GetScreenWidth(), GetScreenHeight());
        }

        drawWindow();
    }

    CloseWindow();
}

void FBEditor::drawWindow()
{
    BeginDrawing();

    ClearBackground(GetColor(GuiGetStyle(GuiControl::DEFAULT, GuiDefaultProperty::BACKGROUND_COLOR)));

    // search panel
    drawSearchPanel(0, 0, search_panel_width, search_panel_height);
    // view panel
    drawViewPanel(search_panel_width, 0, view_panel_width, view_panel_height);

    EndDrawing();
}

void FBEditor::drawSearchPanel(float x, float y, float w, float h)
{
    GuiDrawRectangle(Rectangle{ x, y, w, h }, 4, GetColor(GuiGetStyle(GuiControl::DEFAULT, BORDER_COLOR_NORMAL)), GetColor(GuiGetStyle(GuiControl::DEFAULT, GuiDefaultProperty::BACKGROUND_COLOR)));
    if (GuiButton(Rectangle{ x + 8, y + 8, 100, 100 }, "#30#button"))
        cout << "button!" << endl;

    // search label
    GuiSetStyle(GuiControl::LABEL, GuiControlProperty::TEXT_ALIGNMENT, TEXT_ALIGN_CENTER);
    GuiLabel(Rectangle{ x + 8, y + 8, w - 16, 40 }, "#43#search");

    bool search_pressed = GuiTextBox(Rectangle{ x + 8, y + 8 + 40, w - 16 - 40, 40 }, search_text, 256, true);
    search_pressed |= GuiButton(Rectangle{ x + w - 8 - 40, y + 8 + 40, 40, 40 }, "#42#");
    if (search_pressed) performSearch();


}

void FBEditor::drawViewPanel(float x, float y, float w, float h)
{
    GuiDrawRectangle(Rectangle{ x, y, w, h }, 4, GetColor(GuiGetStyle(GuiControl::DEFAULT, BORDER_COLOR_NORMAL)), GetColor(GuiGetStyle(GuiControl::DEFAULT, GuiDefaultProperty::BACKGROUND_COLOR)));
    GuiLabel(Rectangle{ x + 8, y + 8 + 100, 100, 100 }, "#11#view panel");
}

void FBEditor::updateFrameSizes(float new_width, float new_height)
{
    window_width = new_width;
    window_height = new_height;

    search_panel_width = 480;
    search_panel_height = window_height;

    view_panel_width = window_width - search_panel_width;
    view_panel_height = window_height;

    cout << "new window size: " << window_width << " " << window_height << endl;
}

void FBEditor::performSearch()
{
    cout << "searching for '" << search_text << "'" << endl;
    cout << "NOT IMPLEMENTED" << endl;
}

FBEditor::FBEditor(string database_path)
{
	window_width = 0;
	window_height = 0;

	database = NULL;
	if (!database_path.empty())
		database = new FBDatabase(database_path);

    search_text = new char[256] {'\0'};

	initWindow();
}
