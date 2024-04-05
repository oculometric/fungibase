#pragma once

#include <string>

#include "database.h"
#include "gui.h"
#include "raygui.h"

using namespace std;

class FBEditor
{
private:

	Rectangle window_rect;
	
	Rectangle search_panel_rect;
	FBGuiPanel search_panel;
	FBGuiLabel search_label;
	FBGuiTextBox search_box;
	FBGuiButton search_button;

	Rectangle view_panel_rect;

	Rectangle database_panel_rect;
	FBGuiPanel database_panel;
	FBGuiLabel database_label;
	FBGuiLabel database_file_label;
	FBGuiTextBox database_file_box;
	FBGuiButton database_file_button;
	FBGuiButton database_overwrite_button;
	FBGuiButton database_reload_button;
	FBGuiButton database_load_button;


	float view_panel_width = 0;
	float view_panel_height = 0;

	FBDatabase* database = NULL;

	FBTaxon* current_taxon = NULL;
	char* description_text = NULL;
	bool is_viewing_child_taxa = false;
	
	int modal_state = -1;
	string modal_title;
	string modal_message;
	string modal_options;
	void (FBEditor::* modal_callback)(int);

	void initWindow();
	void drawWindow();

	void updateSearchPanelFrames(float x, float y, float w, float h);
	void drawSearchPanel();

	void drawViewPanel(float x, float y, float w, float h);
	
	void updateDatabasePanelFrames(float x, float y, float w, float h);
	void drawDatabasePanel();

	void updateFrameSizes(float new_width, float new_height); // TODO: update all UI element sizes here too
	void performSearch();
	void moveToTaxon(FBTaxon* taxon);
	void triggerModal(string title, string message, string options, void (FBEditor::* callback)(int));

	void unloadDatabaseCallback(int option);
	void flushDatabaseCallback(int option);
	void reloadDatabaseCallback(int option);

public:
	FBEditor(string database_path = "");
};