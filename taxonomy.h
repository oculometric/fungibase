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

struct FBSource
{
	unsigned int index = 0;
	string link = "";
	string description = "";
	string date_accessed = "";
};

inline bool operator<(const FBSource& a, const FBSource& b)
{
	return a.index < b.index;
}

struct FBTaxon
{
	string name = "Unnamed taxon";
	FBTaxonLevel level = (FBTaxonLevel)(-1);
	string description = "";

	set<FBTaxon*> sub_taxa;
	FBTaxon* parent_taxon = NULL;

	set<FBSource> sources;
};

struct FBFungus;