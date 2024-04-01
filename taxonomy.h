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
	string name = "Unnamed taxon";
	FBTaxonLevel level = (FBTaxonLevel)(-1);
	string description = "";

	vector<FBTaxon*> sub_taxa;
	FBTaxon* parent_taxa = NULL;
};

struct FBFungus;