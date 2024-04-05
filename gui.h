#pragma once

#include <string>

#include "raygui.h"

using namespace std;

class FBGuiElement
{
public:
	Rectangle rect;
	bool enabled = true;

	virtual void draw() abstract;
};

class FBGuiTextElement : public FBGuiElement
{
public:
	GuiTextAlignment horizontal_align = GuiTextAlignment::TEXT_ALIGN_CENTER;
	GuiTextAlignmentVertical vertical_align = GuiTextAlignmentVertical::TEXT_ALIGN_MIDDLE;
	string text = "text";
	GuiIconName icon = GuiIconName::ICON_NONE;
};

class FBGuiLabel : public FBGuiTextElement
{
public:
	void draw() override;
};

class FBGuiButton : public FBGuiTextElement
{
private:
	bool pressed = false;
public:
	bool wasPressed();

	void draw() override;
};

class FBGuiTextBox : public FBGuiTextElement
{
private:
	char* buffer = NULL;
	bool focussed = false;
	bool focus_lost = false;
public:
	bool wasFocusLost();

	void draw() override;

	FBGuiTextBox();

	~FBGuiTextBox();
};

class FBGuiListView : public FBGuiElement
{
private:
	int result = -1;
	int scroll_index = 0;
	int active_index = 0;
public:
	string options = "";

	int getState();

	void draw() override;
};

class FBGuiMessageModal : public FBGuiElement
{
protected:
	int result = -1;
public:
	string title = "title";
	string message = "message";
	string options = "";

	int getState();

	void draw() override;
};

class FBGuiTextModal : public FBGuiMessageModal
{
private:
	char* buffer = NULL;
public:
	bool secret_view_active = false;

	string getText();

	void draw() override;

	FBGuiTextModal();

	~FBGuiTextModal();
};