#include "database.h"

FBDatabase::FBDatabase(string path)
{
	file_address = path;
}

FBDatabase::~FBDatabase()
{
	if (is_open && top_level) flush();
}
