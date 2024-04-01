#pragma once

#include <string>
#include <set>

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

	set<FBTaxon*> sub_taxa;
	FBTaxon* parent_taxon = NULL;
};

struct FBFungus;