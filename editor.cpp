#include "editor.h"

#include <iostream>

#include "styling.h"
#include "fungi.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#define TEXT_LABEL_HEIGHT 24
#define BORDER_OFFSET 8
#define BORDER_OFFSET_2 BORDER_OFFSET * 2

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
    if (GuiButton(Rectangle{ x + 8, y + 100, 100, 100 }, "#30#button"))
        cout << "button!" << endl;

    // search label
    GuiSetStyle(GuiControl::LABEL, GuiControlProperty::TEXT_ALIGNMENT, TEXT_ALIGN_CENTER);
    GuiSetStyle(GuiControl::DEFAULT, GuiDefaultProperty::TEXT_ALIGNMENT_VERTICAL, TEXT_ALIGN_MIDDLE);
    GuiLabel(Rectangle{ x + BORDER_OFFSET, y + BORDER_OFFSET, w - BORDER_OFFSET_2, 40 }, "#43#search");

    bool search_pressed = GuiButton(Rectangle{ x + w - BORDER_OFFSET - 40, y + BORDER_OFFSET + 40, 40, 40 }, "#42#");
    bool search_box_interacted = GuiTextBox(Rectangle{ x + BORDER_OFFSET, y + BORDER_OFFSET + 40, w - BORDER_OFFSET_2 - 40, 40 }, search_text, 256, search_box_focussed);
    if (search_box_interacted && !search_box_focussed) search_box_focussed = true;
    else if (search_box_interacted && search_box_focussed) { search_pressed = true; search_box_focussed = false; }
    if (search_pressed) performSearch();


}

void FBEditor::drawViewPanel(float x, float y, float w, float h)
{
    GuiDrawRectangle(Rectangle{ x, y, w, h }, 4, GetColor(GuiGetStyle(GuiControl::DEFAULT, BORDER_COLOR_NORMAL)), GetColor(GuiGetStyle(GuiControl::DEFAULT, GuiDefaultProperty::BACKGROUND_COLOR)));
    // view label
    GuiSetStyle(GuiControl::LABEL, GuiControlProperty::TEXT_ALIGNMENT, TEXT_ALIGN_CENTER);
    GuiSetStyle(GuiControl::DEFAULT, GuiDefaultProperty::TEXT_ALIGNMENT_VERTICAL, TEXT_ALIGN_MIDDLE);
    float y_offset = y;
    string taxon_name = "no taxon";
    if (current_taxon) taxon_name = current_taxon->name;
    if (current_taxon && (current_taxon->level == FBTaxonLevel::SPECIES || current_taxon->level == FBTaxonLevel::SUBSPECIES))
    {
        taxon_name = current_taxon->parent_taxon->name + ' ' + taxon_name;
    }
    GuiLabel(Rectangle{ x + BORDER_OFFSET, y_offset + BORDER_OFFSET, w - BORDER_OFFSET_2, 40 }, (string("#176# view - ") + taxon_name).c_str());
    if (current_taxon && current_taxon->parent_taxon && GuiButton(Rectangle{ x + w - 96 - BORDER_OFFSET, y_offset + BORDER_OFFSET, 96, 40 }, "parent")) moveToTaxon(current_taxon->parent_taxon);
    y_offset += 40 + BORDER_OFFSET;

    if (!current_taxon) return;

    // taxon level label
    GuiLabel(Rectangle{ x + 8, y_offset, w - 16, TEXT_LABEL_HEIGHT }, (string("(") + getDescription(current_taxon->level) + string(")")).c_str());
    y_offset += TEXT_LABEL_HEIGHT;

    // description box
    GuiSetStyle(GuiControl::LABEL, GuiControlProperty::TEXT_ALIGNMENT, TEXT_ALIGN_RIGHT);
    GuiSetStyle(GuiControl::DEFAULT, GuiDefaultProperty::TEXT_ALIGNMENT_VERTICAL, TEXT_ALIGN_TOP);
    GuiLabel(Rectangle{ x + BORDER_OFFSET, y_offset, 120, TEXT_LABEL_HEIGHT }, "description:");
    GuiSetStyle(GuiControl::LABEL, GuiControlProperty::TEXT_ALIGNMENT, TEXT_ALIGN_LEFT);
    GuiTextBox(Rectangle{ x + BORDER_OFFSET + 120, y_offset, w - BORDER_OFFSET_2 - 120, 240 }, description_text, 4096, false);
    y_offset += 240;

    if (current_taxon->level != FBTaxonLevel::SPECIES && current_taxon->level != FBTaxonLevel::SUBSPECIES) return;
    FBFungus* current_fungus = (FBFungus*)current_taxon;

    // feeding type
    GuiSetStyle(GuiControl::LABEL, GuiControlProperty::TEXT_ALIGNMENT, TEXT_ALIGN_RIGHT);
    GuiLabel(Rectangle{ x + BORDER_OFFSET, y_offset, 120, TEXT_LABEL_HEIGHT }, "feeding:");
    GuiSetStyle(GuiControl::LABEL, GuiControlProperty::TEXT_ALIGNMENT, TEXT_ALIGN_LEFT);
    GuiLabel(Rectangle{ x + BORDER_OFFSET + 120, y_offset, w - BORDER_OFFSET_2 - 120, TEXT_LABEL_HEIGHT }, getDescription(current_fungus->feeding_type).c_str());
    y_offset += TEXT_LABEL_HEIGHT;

    // tags list
    GuiSetStyle(GuiControl::LABEL, GuiControlProperty::TEXT_ALIGNMENT, TEXT_ALIGN_RIGHT);
    GuiLabel(Rectangle{ x + BORDER_OFFSET, y_offset, 120, TEXT_LABEL_HEIGHT }, "tags:");
    string tags_string = "";
    for (FBTag tag : current_fungus->tags)
        tags_string += getDescription(tag) + "; ";
    GuiSetStyle(GuiControl::LABEL, GuiControlProperty::TEXT_ALIGNMENT, TEXT_ALIGN_LEFT);
    GuiLabel(Rectangle{ x + BORDER_OFFSET + 120, y_offset, w - BORDER_OFFSET_2 - 120, TEXT_LABEL_HEIGHT }, tags_string.c_str());
    y_offset += TEXT_LABEL_HEIGHT;

    // TODO: other fungus info
    // TODO: access to parent taxa
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

void FBEditor::moveToTaxon(FBTaxon* taxon)
{
    current_taxon = taxon;
    if (!current_taxon) return;
    if (!description_text) description_text = new char[4096] {'\0'};
    memset(description_text, '\0', 4096);
    memcpy(description_text, (void*)(current_taxon->description.c_str()), current_taxon->description.length());
}

FBEditor::FBEditor(string database_path)
{
	window_width = 0;
	window_height = 0;

	database = NULL;
	if (!database_path.empty())
		database = new FBDatabase(database_path);

    database->reload();
    moveToTaxon(database->find("muscaria", FBTaxonLevel::SPECIES));

    search_text = new char[256] {'\0'};

	initWindow();
}
