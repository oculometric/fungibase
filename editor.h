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



	Rectangle view_panel;
	Rectangle database_panel;

	float view_panel_width = 0;
	float view_panel_height = 0;

	float database_panel_width = 0;
	float database_panel_height = 0;

	FBDatabase* database = NULL;
	char* database_address = NULL;

	FBTaxon* current_taxon = NULL;
	char* search_text = NULL;
	bool search_box_focussed = false;
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
	void drawDatabasePanel(float x, float y, float w, float h);
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