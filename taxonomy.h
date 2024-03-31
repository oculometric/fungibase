#pragma once

#include <string>
#include <vector>

using namespace std;

enum FBTaxonLevel
{
	KINGDOM,
	PHYLUM,
	SUBPHYLUM,
	CLASS,
	SUBCLASS,
	ORDER,
	FAMILY,
	GENUS,
	SPECIES,
	SUBSPECIES
};

string getDescription(FBTaxonLevel);

struct FBTaxon
{
	string name;
	FBTaxonLevel level;
	string description;

	vector<FBTaxon*> sub_taxa;
	FBTaxon* parent_taxa;
};

string serialise(const FBTaxon&);
void deserialise(const string&, FBTaxon&);

struct FBFungus;