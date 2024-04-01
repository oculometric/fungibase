#pragma once

#include "taxonomy.h"

// TODO: convert all vectors to sets (non-ordered, fast-searchable, unique-item)

class FBDatabase
{
private:
	FBTaxon* top_level;

	vector<FBFungus*> all_fungi_cards;

	string file_address;
	bool is_open;

	void unloadDatabase();

public:
	FBDatabase(string path);

	// TODO: access functions

	bool reload();
	vector<FBFungus*> search(bool(*evaluator)(const FBFungus&));
	bool insertSpecies(const FBFungus);
	bool flush();

	~FBDatabase();
};