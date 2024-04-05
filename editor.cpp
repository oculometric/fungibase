#include "editor.h"

#include <iostream>

#include "styling.h"
#include "fungi.h"
#include "raygui.h"

#define TEXT_LABEL_HEIGHT 24
#define BORDER_OFFSET 8
#define BORDER_OFFSET_2 BORDER_OFFSET * 2

void FBEditor::initWindow()
{
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    SetTargetFPS(30);
    InitWindow(window_rect.width, window_rect.height, "fungibase");
    
    for (int i = 0; i < STYLE_PROPS_COUNT; i++)
        GuiSetStyle(styleProperties[i].controlId, styleProperties[i].propertyId, styleProperties[i].propertyValue);

    window_rect.width = GetMonitorWidth(0) / 2;
    window_rect.height = GetMonitorHeight(0) / 2;
    SetWindowSize(window_rect.width, window_rect.height);
    SetWindowPosition(window_rect.width / 2, window_rect.height / 2);

    // search panel setup
    search_panel.border = GetColor(GuiGetStyle(GuiControl::DEFAULT, BORDER_COLOR_NORMAL));
    search_panel.background = GetColor(GuiGetStyle(GuiControl::DEFAULT, GuiDefaultProperty::BACKGROUND_COLOR));
    search_label.text = "search";
    search_label.icon = GuiIconName::ICON_LENS_BIG;
    search_button.icon = GuiIconName::ICON_LENS;

    // view panel setup

    // dataase panel setup
    database_panel.border = GetColor(GuiGetStyle(GuiControl::DEFAULT, BORDER_COLOR_NORMAL));
    database_panel.background = GetColor(GuiGetStyle(GuiControl::DEFAULT, GuiDefaultProperty::BACKGROUND_COLOR));
    database_label.text = "database";
    database_label.icon = GuiIconName::ICON_GRID_FILL;
    database_file_label.text = "file:";
    database_file_label.horizontal_align = GuiTextAlignment::TEXT_ALIGN_RIGHT;
    database_file_button.icon = GuiIconName::ICON_FOLDER_FILE_OPEN;
    database_overwrite_button.text = "overwrite stored";
    database_overwrite_button.icon = GuiIconName::ICON_STEP_INTO;
    database_reload_button.text = "reload from disk";
    database_reload_button.icon = GuiIconName::ICON_STEP_OUT;

    // mainloop
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
    
    if (modal_state < 3)
    {
        ClearBackground(GetColor(GuiGetStyle(GuiControl::DEFAULT, GuiDefaultProperty::BACKGROUND_COLOR)));
        // search panel
        drawSearchPanel();
        // view panel
        drawViewPanel(search_panel_rect.width, 0, view_panel_width, view_panel_height);
        // database panel
        drawDatabasePanel();
    }
    else
    {
        int value = GuiMessageBox(Rectangle{ window_rect.width / 4, window_rect.height / 4, window_rect.width / 2, window_rect.height / 2 }, modal_title.c_str(), modal_message.c_str(), modal_options.c_str());
        if (value >= 0)
        {
            modal_state = -1;
            (this->*modal_callback)(value);
        }
    }

    if (modal_state > 0 && modal_state < 3)
        modal_state++;

    EndDrawing();
}

void FBEditor::updateSearchPanelFrames(float x, float y, float w, float h)
{
    search_panel.rect = Rectangle{ x, y, w, h };
    search_label.rect = Rectangle{ x + BORDER_OFFSET, y + BORDER_OFFSET, w - BORDER_OFFSET_2, 40 };
    search_box.rect = Rectangle{ x + BORDER_OFFSET, y + BORDER_OFFSET + 40, w - BORDER_OFFSET_2 - 40, 40 };
    search_button.rect = Rectangle{ x + w - BORDER_OFFSET - 40, y + BORDER_OFFSET + 40, 40, 40 };
}

void FBEditor::drawSearchPanel()
{
    search_panel.draw();

    search_label.draw();
    search_box.draw();
    search_button.draw();

    if (search_button.wasPressed() || search_box.wasFocusLost()) performSearch();

    // TODO: rest of the search panel
}

void FBEditor::drawViewPanel(float x, float y, float w, float h)
{
    //GuiDrawRectangle(Rectangle{ x, y, w, h }, 4, GetColor(GuiGetStyle(GuiControl::DEFAULT, BORDER_COLOR_NORMAL)), GetColor(GuiGetStyle(GuiControl::DEFAULT, GuiDefaultProperty::BACKGROUND_COLOR)));
    // view label
    GuiSetStyle(GuiControl::LABEL, GuiControlProperty::TEXT_ALIGNMENT, TEXT_ALIGN_CENTER);
    GuiSetStyle(GuiControl::DEFAULT, GuiDefaultProperty::TEXT_ALIGNMENT_VERTICAL, TEXT_ALIGN_MIDDLE);
    float y_offset = y;
    string taxon_name = "no taxon";
    if (current_taxon) taxon_name = current_taxon->name;
    if (current_taxon && isSpecies(current_taxon))
    {
        taxon_name = current_taxon->parent_taxon->name + ' ' + taxon_name;
    }
    GuiLabel(Rectangle{ x + BORDER_OFFSET, y_offset + BORDER_OFFSET, w - BORDER_OFFSET_2, 40 }, (string("#176# view - ") + taxon_name).c_str());
    // jump to parent taxon
    if (current_taxon && current_taxon->parent_taxon && GuiButton(Rectangle{ x + BORDER_OFFSET, y_offset + BORDER_OFFSET, 96, 40 }, "#117#parent"))
    {
        moveToTaxon(current_taxon->parent_taxon);
        is_viewing_child_taxa = false;
    }
    // jump to child taxon
    if (current_taxon && current_taxon->sub_taxa.size() > 0 && GuiButton(Rectangle{ x + w - 96 - BORDER_OFFSET, y_offset + BORDER_OFFSET, 96, 40 }, "#116#child"))
    {
        is_viewing_child_taxa = !is_viewing_child_taxa;
    }
    y_offset += 40 + BORDER_OFFSET;

    if (!current_taxon) return;

    // child taxon selection
    if (is_viewing_child_taxa)
    {
        if (current_taxon->sub_taxa.empty())
        {
            is_viewing_child_taxa = false;
            return;
        }
        int item = -1;
        int scroll = 0;
        string child_taxa = "";
        for (FBTaxon* child : current_taxon->sub_taxa)
        {
            if (child->level == FBTaxonLevel::SUBSPECIES)
            {
                child_taxa += current_taxon->parent_taxon->name + " ";
            }
            if (isSpecies(child))
            {
                child_taxa += current_taxon->name + " ";
            }
            child_taxa += child->name + ";";
        }
        child_taxa.pop_back();
        GuiListView(Rectangle{ x + BORDER_OFFSET, y_offset, w - BORDER_OFFSET_2, h - y_offset - BORDER_OFFSET }, child_taxa.c_str(), &scroll, &item);
        if (item >= 0 && item < current_taxon->sub_taxa.size())
        {
            is_viewing_child_taxa = false;
            auto child = current_taxon->sub_taxa.begin();
            for (int i = 0; i < item; i++) child++;
            moveToTaxon(*(child));
        }
        return;
    }


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

    if (!isSpecies(current_taxon)) return;
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
    // TODO: access to child taxa


}

void FBEditor::updateDatabasePanelFrames(float x, float y, float w, float h)
{
    database_panel.rect = Rectangle{ x, y, w, h };
    float y_offset = y;
    database_label.rect = Rectangle{ x + BORDER_OFFSET, y_offset + BORDER_OFFSET, w - BORDER_OFFSET_2, 40 };
    y_offset += 40 + BORDER_OFFSET;
    database_file_label.rect = Rectangle{ x + BORDER_OFFSET, y_offset, 120, TEXT_LABEL_HEIGHT };
    database_file_box.rect = Rectangle{ x + BORDER_OFFSET + 120, y_offset, w - BORDER_OFFSET_2 - 120 - TEXT_LABEL_HEIGHT, TEXT_LABEL_HEIGHT };
    database_file_button.rect = Rectangle{ w - BORDER_OFFSET - TEXT_LABEL_HEIGHT, y_offset, TEXT_LABEL_HEIGHT, TEXT_LABEL_HEIGHT };
    y_offset += TEXT_LABEL_HEIGHT;
    y_offset += TEXT_LABEL_HEIGHT;
    database_overwrite_button.rect = Rectangle{ x + BORDER_OFFSET, y_offset, (w / 2) - BORDER_OFFSET, TEXT_LABEL_HEIGHT };
    database_reload_button.rect = Rectangle{ x + (w / 2), y_offset, (w / 2) - BORDER_OFFSET, TEXT_LABEL_HEIGHT };
    y_offset += TEXT_LABEL_HEIGHT;
    database_load_button.rect = Rectangle{ x + BORDER_OFFSET, y_offset, w - BORDER_OFFSET_2, TEXT_LABEL_HEIGHT };
}

void FBEditor::drawDatabasePanel()
{
    // update values
    bool database_currently_loaded = database && database->isOpen();
    database_file_box.enabled = !database_currently_loaded;
    database_file_button.enabled = !database_currently_loaded;
    database_overwrite_button.enabled = database_currently_loaded;
    database_reload_button.enabled = database_currently_loaded;
    database_load_button.text = database_currently_loaded ? "unload from memory" : "load from disk";
    database_load_button.icon = database_currently_loaded ? GuiIconName::ICON_ZOOM_SMALL : GuiIconName::ICON_ZOOM_ALL;

    // draw elements
    database_panel.draw();
    database_label.draw();
    database_file_label.draw();
    database_file_box.draw();
    database_file_button.draw();
    database_overwrite_button.draw();
    database_reload_button.draw();
    database_load_button.draw();

    // respond to input
    if (database_file_button.wasPressed())
    {
        // TODO: show file select dialog
        cout << "TODO file selector" << endl;
    }

    // save/load buttons
    if (database_overwrite_button.wasPressed())
        triggerModal("WARNING", "this operation will overwrite the copy of \nthe database stored on disk \nwith the version in memory. \nproceed?", "no;yes", &FBEditor::flushDatabaseCallback);
    if (database_reload_button.wasPressed())
        triggerModal("WARNING", "this operation will overwrite the copy of \nthe database stored in memory with the\n version from the disk. \nunsaved modifications will be lost! \nproceed?", "no;yes", &FBEditor::reloadDatabaseCallback);
    
    // load/unload button
    if (database_load_button.wasPressed())
    {
        if (database_currently_loaded)
            triggerModal("WARNING", "this operation will unload the database from memory.\nunsaved changes will be lost!\nproceed?", "no;yes", &FBEditor::unloadDatabaseCallback);
        else
        {
            if (database != NULL)
            {
                delete database;
                database = NULL;
                current_taxon = NULL;
            }
            database = new FBDatabase(database_file_box.getText());
            database->reload();
            if (!database->isOpen())
            {
                delete database;
                database = NULL;
                current_taxon = NULL;
            }
            else
                moveToTaxon(database->find("Fungi", FBTaxonLevel::KINGDOM));
        }
    }
}

void FBEditor::unloadDatabaseCallback(int option)
{
    if (option == 1) { }    // no
    else if (option == 2)   // yes
    {
        is_viewing_child_taxa = false;
        delete database;
        database = NULL;
        current_taxon = NULL;
    }
}

void FBEditor::reloadDatabaseCallback(int option)
{
    if (option == 1) { }    // no
    else if (option == 2)   // yes
    {
        is_viewing_child_taxa = false;
        if (!database) return;
        database->reload();
        if (database->isOpen())
            moveToTaxon(database->find("Fungi", FBTaxonLevel::KINGDOM));
    }
}

void FBEditor::flushDatabaseCallback(int option)
{
    if (option == 1) {}    // no
    else if (option == 2)   // yes
    {
        if (!database) return;
        database->flush();
    }
}

void FBEditor::updateFrameSizes(float new_width, float new_height)
{
    window_rect = Rectangle{ 0,0, new_width, new_height };

    search_panel_rect = Rectangle{ 0, 0, 480, window_rect.height - 240 };
    updateSearchPanelFrames(search_panel_rect.x, search_panel_rect.y, search_panel_rect.width, search_panel_rect.height);

    view_panel_width = window_rect.width - search_panel_rect.width;
    view_panel_height = window_rect.height;

    database_panel_rect = Rectangle{ 0, search_panel_rect.height, search_panel_rect.width, window_rect.height - search_panel_rect.height };
    updateDatabasePanelFrames(database_panel_rect.x, database_panel_rect.y, database_panel_rect.width, database_panel_rect.height);

    cout << "new window size: " << window_rect.width << " " << window_rect.height << endl;
}

void FBEditor::performSearch()
{
    cout << "searching for '" << search_box.getText() << "'" << endl;
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
	if (!database_path.empty())
		database = new FBDatabase(database_path);
    database_file_box.setText(database_path.c_str());

    database->reload();
    moveToTaxon(database->find("Fungi", FBTaxonLevel::KINGDOM));

	initWindow();
}

void FBEditor::triggerModal(string title, string message, string options, void (FBEditor::* callback)(int))
{
    modal_title = title;
    modal_message = message;
    modal_options = options;
    modal_callback = callback;

    modal_state = 1;
}