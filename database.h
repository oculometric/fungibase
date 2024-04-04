#pragma once

#include <map>
#include "taxonomy.h"

class FBDatabase
{
private:
	FBTaxon* top_level;

	map<string, FBFungus*> fungi_index;

	string file_address;
	bool is_open;

	void unloadDatabase();
	bool checkIsInHeirarchy(FBTaxon*);

public:
	FBDatabase(string path);

	bool reload();
	set<FBFungus*> search(bool(*evaluator)(const FBFungus&));
	FBFungus* findBinomial(string binomial_name);
	FBTaxon* find(string name, FBTaxonLevel level);
	set<FBTaxon*> list(FBTaxon*, FBTaxonLevel);
	bool insert(const FBTaxon&, FBTaxon*);
	bool remove(FBTaxon*);
	bool flush();

	bool isOpen();

	static string getBinomialName(const FBTaxon&);
	
	~FBDatabase();
};