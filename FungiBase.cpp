// FungiBase.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>

#include "serialise.h"
#include "database.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

int main()
{
    cout << "Hello World!\n";

    FBDatabase database("database.json");
    cout << database.reload() << endl;
    cout << database.flush() << endl;

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(400, 400, "test");
    GuiLoadStyle("../style_lavanda.rgs");

    while (!WindowShouldClose())
    {
        BeginDrawing();

        ClearBackground(GetColor(0x5b5b81ff /*GuiGetStyle(DEFAULT, BACKGROUND_COLOR)*/));
        if (GuiButton(Rectangle{ 10, 10, 100, 100 }, "#30#button"))
            cout << "button!" << endl;

        if (IsWindowResized())
            cout << "resize!" << endl;

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
