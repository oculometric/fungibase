#include "database.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include "serialise.h"

void deleteInsideTaxon(FBTaxon* t)
{
	if (!t) return;
	for (FBTaxon* t_sub : t->sub_taxa) deleteInsideTaxon(t_sub);
	delete t;
}

void FBDatabase::unloadDatabase()
{
	if (top_level)
	{
		deleteInsideTaxon(top_level);
		top_level = NULL;
	}
}

FBDatabase::FBDatabase(string path)
{
	file_address = path;
}

bool FBDatabase::reload()
{
	ifstream file;
	file.open(file_address);
	if (!file.is_open())
	{
		is_open = false;
		return false;
	}

	unloadDatabase();
	try
	{
		top_level = new FBTaxon();
		string entire_file;
		stringstream buffer;
		buffer << file.rdbuf();
		file.close();

		if (buffer.str().empty())
		{
			is_open = false;
			return false;
		}

		FBJsonObject obj = objectifyJson(buffer.str());
		deserialise(obj, *top_level);
		is_open = true;
		return true;
	}
	catch (exception e)
	{
		cout << "something went wrong while loading the database: " << e.what() << endl;
		is_open = false;
		file.close();
		return false;
	}
}

bool FBDatabase::flush()
{
	if (top_level == NULL || !is_open) return false;

	ofstream file;
	file.open(file_address);
	if (!file.is_open()) return false;

	
	file << jsonifyObject(serialise(*top_level));

	file.close();
}

FBDatabase::~FBDatabase()
{
	if (is_open && top_level) flush();
	unloadDatabase();
	is_open = false;
}
