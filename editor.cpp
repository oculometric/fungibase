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
    view_panel.border = GetColor(GuiGetStyle(GuiControl::DEFAULT, BORDER_COLOR_NORMAL));
    view_panel.background = GetColor(GuiGetStyle(GuiControl::DEFAULT, GuiDefaultProperty::BACKGROUND_COLOR));
    view_label.icon = GuiIconName::ICON_TEXT_NOTES;
    view_parent_button.text = "parent";
    view_parent_button.icon = GuiIconName::ICON_ARROW_UP;
    view_child_button.text = "child";
    view_child_button.icon = GuiIconName::ICON_ARROW_DOWN;
    view_add_button.text = "new taxon";
    view_add_button.icon = GuiIconName::ICON_FOLDER_ADD;
    view_description_label.horizontal_align = GuiTextAlignment::TEXT_ALIGN_RIGHT;
    view_description_label.text = "description:";
    view_description_box.horizontal_align = GuiTextAlignment::TEXT_ALIGN_LEFT;
    view_description_box.vertical_align = GuiTextAlignmentVertical::TEXT_ALIGN_TOP;
    view_description_box.editable = false;
    view_feeding_label.horizontal_align = GuiTextAlignment::TEXT_ALIGN_RIGHT;
    view_feeding_label.text = "feeding:";
    view_feeding_label_2.horizontal_align = GuiTextAlignment::TEXT_ALIGN_LEFT;
    view_tags_label.horizontal_align = GuiTextAlignment::TEXT_ALIGN_RIGHT;
    view_tags_label.text = "tags:";
    view_tags_label_2.horizontal_align = GuiTextAlignment::TEXT_ALIGN_LEFT;

    // database panel setup
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

    modal_m.enabled = false;
    modal_t.enabled = false;
    modal_i.enabled = false;

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
    
    if (modal_state < 7)
    {
        ClearBackground(GetColor(GuiGetStyle(GuiControl::DEFAULT, GuiDefaultProperty::BACKGROUND_COLOR)));
        // search panel
        drawSearchPanel();
        // view panel
        drawViewPanel();
        // database panel
        drawDatabasePanel();
    }
    else
    {
        if (modal_m.enabled)
        {
            modal_m.rect = Rectangle{ window_rect.width / 4, window_rect.height / 4, window_rect.width / 2, window_rect.height / 2 };
            modal_m.draw();
            if (modal_m.getState() >= 0)
            {
                modal_state = -1;
                modal_m.enabled = false;
                (this->*modal_callback)(modal_m.getState(), "");
            }
        }
        else if (modal_t.enabled)
        {
            modal_t.rect = Rectangle{ window_rect.width / 4, window_rect.height / 4, window_rect.width / 2, window_rect.height / 2 };
            modal_t.secret_view_active = true;
            modal_t.draw();
            if (modal_t.getState() >= 0)
            {
                modal_state = -1;
                modal_t.enabled = false;
                (this->*modal_callback)(modal_t.getState(), modal_t.getText());
            }
        }
        else if (modal_i.enabled)
        {
            modal_i.rect = Rectangle{ window_rect.width / 4, window_rect.height / 4, window_rect.width / 2, window_rect.height / 2 };
            modal_i.draw();
            if (modal_i.getState() >= 0)
            {
                modal_state = -1;
                modal_i.enabled = false;
            }
        }
    }

    if (modal_state > 0 && modal_state < 7)
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

void FBEditor::updateViewPanelFrames(float x, float y, float w, float h)
{
    view_panel.rect = Rectangle{ x, y, w, h };
    float y_offset = y;
    view_label.rect = Rectangle{ x + BORDER_OFFSET, y_offset + BORDER_OFFSET, w - BORDER_OFFSET_2, 40 };
    view_parent_button.rect = Rectangle{ x + BORDER_OFFSET, y_offset + BORDER_OFFSET, 96, 40 };
    view_child_button.rect = Rectangle{ x + w - 96 - BORDER_OFFSET, y_offset + BORDER_OFFSET, 96, 40 };
    y_offset += 40 + BORDER_OFFSET;
    view_child_dropdown.rect = Rectangle{ x + BORDER_OFFSET, y_offset, w - BORDER_OFFSET_2, h - y_offset - BORDER_OFFSET };
    view_level_label.rect = Rectangle{ x + BORDER_OFFSET, y_offset, w - 16, TEXT_LABEL_HEIGHT };
    view_add_button.rect = Rectangle{ x + w - 120 - BORDER_OFFSET, y_offset, 120, TEXT_LABEL_HEIGHT };
    y_offset += TEXT_LABEL_HEIGHT;
    view_description_label.rect = Rectangle{ x + BORDER_OFFSET, y_offset, 120, TEXT_LABEL_HEIGHT };
    view_description_box.rect = Rectangle{ x + BORDER_OFFSET + 120, y_offset, w - BORDER_OFFSET_2 - 120, 240 };
    y_offset += 240;
    view_feeding_label.rect = Rectangle{ x + BORDER_OFFSET, y_offset, 120, TEXT_LABEL_HEIGHT };
    view_feeding_label_2.rect = Rectangle{ x + BORDER_OFFSET + 120, y_offset, w - BORDER_OFFSET_2 - 120, TEXT_LABEL_HEIGHT };
    y_offset += TEXT_LABEL_HEIGHT;
    view_tags_label.rect = Rectangle{ x + BORDER_OFFSET, y_offset, 120, TEXT_LABEL_HEIGHT };
    view_tags_label_2.rect = Rectangle{ x + BORDER_OFFSET + 120, y_offset, w - BORDER_OFFSET_2 - 120, TEXT_LABEL_HEIGHT };
    y_offset += TEXT_LABEL_HEIGHT;
}

void FBEditor::drawViewPanel()
{
    view_parent_button.enabled = current_taxon && current_taxon->parent_taxon;
    view_child_button.enabled = current_taxon && current_taxon->sub_taxa.size() > 0;
    if (current_taxon && current_taxon->sub_taxa.empty()) is_viewing_child_taxa = false;

    view_panel.draw();
    view_label.draw();
    view_parent_button.draw();
    view_child_button.draw();
    
    if (current_taxon)
    {
        if (is_viewing_child_taxa)
            view_child_dropdown.draw();
        else
        {
            view_level_label.draw();
            view_description_label.draw();
            view_description_box.draw();
            if (current_taxon->level != FBTaxonLevel::SUBSPECIES) view_add_button.draw();

            if (isSpecies(current_taxon))
            {
                FBFungus* current_fungus = (FBFungus*)current_taxon;

                view_feeding_label.draw();
                view_feeding_label_2.draw();

                view_tags_label.draw();
                view_tags_label_2.draw();
            }
        }
    }

    if (!current_taxon) return;

    if (current_taxon->level != FBTaxonLevel::SUBSPECIES && view_add_button.wasPressed() && !is_viewing_child_taxa)
    {
        bool may_skip = current_taxon->level == FBTaxonLevel::PHYLUM || current_taxon->level == FBTaxonLevel::CLASS;
        triggerTextModal("CREATE TAXON", "creating a new " + getDescription((FBTaxonLevel)(current_taxon->level + 1)) + " as a child of " + current_taxon->name, may_skip ? "cancel;confirm;skip to lower level" : "cancel;confirm", &FBEditor::newTaxonCallback);
    }
    if (is_viewing_child_taxa)
    {
        if (view_child_dropdown.getState() >= 0 && view_child_dropdown.getState() < current_taxon->sub_taxa.size())
        {
            is_viewing_child_taxa = false;
            auto child = current_taxon->sub_taxa.begin();
            for (int i = 0; i < view_child_dropdown.getState(); i++) child++;
            moveToTaxon(*(child));
        }
    }
    // jump to parent taxon
    if (current_taxon->parent_taxon && view_parent_button.wasPressed())
    {
        moveToTaxon(current_taxon->parent_taxon);
        is_viewing_child_taxa = false;
    }
    // jump to child taxon
    if (current_taxon->sub_taxa.size() > 0 && view_child_button.wasPressed())
    {
        is_viewing_child_taxa = !is_viewing_child_taxa;
        if (is_viewing_child_taxa)
        {
            string child_taxa = "";
            for (FBTaxon* child : current_taxon->sub_taxa)
            {
                if (child->level == FBTaxonLevel::SUBSPECIES && current_taxon->parent_taxon)
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
            view_child_dropdown.options = child_taxa;
            view_child_dropdown.reset();
        }
    }
    
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
    database_file_box.editable = !database_currently_loaded;
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
        triggerMessageModal("WARNING", "this operation will overwrite the copy of \nthe database stored on disk \nwith the version in memory. \nproceed?", "no;yes", &FBEditor::flushDatabaseCallback);
    if (database_reload_button.wasPressed())
        triggerMessageModal("WARNING", "this operation will overwrite the copy of \nthe database stored in memory with the\n version from the disk. \nunsaved modifications will be lost! \nproceed?", "no;yes", &FBEditor::reloadDatabaseCallback);
    
    // load/unload button
    if (database_load_button.wasPressed())
    {
        if (database_currently_loaded)
            triggerMessageModal("WARNING", "this operation will unload the database from memory.\nunsaved changes will be lost!\nproceed?", "no;yes", &FBEditor::unloadDatabaseCallback);
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

void FBEditor::unloadDatabaseCallback(int option, string _)
{
    if (option == 1) { }    // no
    else if (option == 2)   // yes
    {
        is_viewing_child_taxa = false;
        delete database;
        database = NULL;
        moveToTaxon(NULL);
    }
}

void FBEditor::reloadDatabaseCallback(int option, string _)
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

void FBEditor::flushDatabaseCallback(int option, string _)
{
    if (option == 1) {}    // no
    else if (option == 2)   // yes
    {
        if (!database) return;
        database->flush();
    }
}

void FBEditor::newTaxonCallback(int option, string name)
{
    if (option == 1) {} // cancel
    else if (option == 2) // confirm
    {
        if (name.empty()) return;
        if (!database || !database->isOpen() || !current_taxon) { triggerInfoModal("ERROR", "unable to create new taxon.\n database state is invalid!"); return; }
        if (database->insert(FBTaxon{ name, (FBTaxonLevel)(current_taxon->level + 1), "this taxon lacks a description" }, current_taxon))
        {
        }
        else
        {
            triggerInfoModal("ERROR", "        unable to create new taxon.        \n there may have been a naming collision.");
        }
    }
    else if (option == 3) // one step further down
    {
        triggerTextModal("CREATE TAXON", "creating a new " + getDescription((FBTaxonLevel)(current_taxon->level + 2)) + " as a child of " + current_taxon->name, "cancel;confirm", &FBEditor::newTaxonCallbackDeeper);
    }
}

void FBEditor::newTaxonCallbackDeeper(int option, string name)
{
    if (option == 1) {} // cancel
    else if (option == 2) // confirm
    {
        if (name.empty()) return;
        if (!database || !database->isOpen() || !current_taxon) { triggerInfoModal("ERROR", "unable to create new taxon.\n database state is invalid!"); return; }
        if (database->insert(FBTaxon{ name, (FBTaxonLevel)(current_taxon->level + 2), "this taxon lacks a description" }, current_taxon))
        {

        }
        else
        {
            triggerInfoModal("ERROR", "        unable to create new taxon.        \n there may have been a naming collision.");
        }
    }
}

void FBEditor::updateFrameSizes(float new_width, float new_height)
{
    window_rect = Rectangle{ 0,0, new_width, new_height };

    search_panel_rect = Rectangle{ 0, 0, 480, window_rect.height - 240 };
    updateSearchPanelFrames(search_panel_rect.x, search_panel_rect.y, search_panel_rect.width, search_panel_rect.height);

    view_panel_rect = Rectangle{ search_panel_rect.width, 0, window_rect.width - search_panel_rect.width, window_rect.height };
    updateViewPanelFrames(view_panel_rect.x, view_panel_rect.y, view_panel_rect.width, view_panel_rect.height);

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
    if (!current_taxon)
    {
        view_label.text = "no taxon";
        return;
    }

    string taxon_name = "no taxon";
    if (current_taxon) taxon_name = current_taxon->name;
    if (current_taxon && isSpecies(current_taxon))
    {
        taxon_name = current_taxon->parent_taxon->name + ' ' + taxon_name;
        if (current_taxon->level == FBTaxonLevel::SUBSPECIES)
            taxon_name = current_taxon->parent_taxon->parent_taxon->name + ' ' + taxon_name;
    }
    view_label.text = "view - " + taxon_name;

    view_level_label.text = "(" + getDescription(current_taxon->level) + ")";
    view_description_box.setText(taxon->description.c_str());

    if (!isSpecies(current_taxon)) return;
    FBFungus* current_fungus = (FBFungus*)current_taxon;

    view_feeding_label_2.text = getDescription(current_fungus->feeding_type);

    string tags_string = "";
    for (FBTag tag : current_fungus->tags)
        tags_string += getDescription(tag) + "; ";
    view_tags_label_2.text = tags_string;
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

void FBEditor::triggerMessageModal(string title, string message, string options, void (FBEditor::* callback)(int, string))
{
    if (modal_m.enabled || modal_t.enabled || modal_i.enabled) return;
    modal_m.title = title;
    modal_m.message = message;
    modal_m.options = options;
    modal_m.enabled = true;
    modal_callback = callback;

    modal_state = 1;
}

void FBEditor::triggerTextModal(string title, string message, string options, void (FBEditor::* callback)(int, string))
{
    if (modal_m.enabled || modal_t.enabled || modal_i.enabled) return;
    modal_t.title = title;
    modal_t.message = message;
    modal_t.options = options;
    modal_t.enabled = true;
    modal_t.clearBuffer();
    modal_callback = callback;

    modal_state = 1;
}

void FBEditor::triggerInfoModal(string title, string message)
{
    if (modal_m.enabled || modal_t.enabled || modal_i.enabled) return;
    modal_i.title = title;
    modal_i.message = message;
    modal_i.enabled = true;

    modal_state = 1;
}