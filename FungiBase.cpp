// FungiBase.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>

#include "serialise.h"
#include "database.h"

int main()
{
    cout << "Hello World!\n";

    FBDatabase database("database.json");
    cout << database.reload() << endl;
    cout << database.flush() << endl;
}
