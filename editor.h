#pragma once

#include <string>

#include "database.h"

using namespace std;

class FBEditor
{
private:
	float window_width;
	float window_height;

	float search_panel_width = 0;
	float search_panel_height = 0;

	float view_panel_width = 0;
	float view_panel_height = 0;

	FBDatabase* database;

	char* search_text;

	void initWindow();
	void drawWindow();
	void drawSearchPanel(float x, float y, float w, float h);
	void drawViewPanel(float x, float y, float w, float h);
	void updateFrameSizes(float new_width, float new_height);
	void performSearch();

public:
	FBEditor(string database_path = "");
};