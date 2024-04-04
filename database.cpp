#include "database.h"

#include <queue>
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
	fungi_index.clear();
}

bool FBDatabase::checkIsInHeirarchy(FBTaxon* taxon)
{
	FBTaxon* parent = taxon;
	int iterations = 100;
	while (parent != top_level)
	{
		if (parent == NULL) return false;
		if (iterations == 0) return false;
		parent = parent->parent_taxon;
		iterations--;
	}
	return true;
}

inline string lowercase(const string& str)
{
	string s;
	s.reserve(str.length());
	for (char c : str) s.push_back(tolower(c));
	return s;
}

inline string firstUppercaseOnly(const string& str)
{
	string s = lowercase(str);
	if (s.length() > 0)
		s[0] = toupper(s[0]);
	return s;
}

string FBDatabase::getBinomialName(const FBTaxon& taxon)
{
	string bin_name = lowercase(taxon.name);
	if (taxon.level == FBTaxonLevel::SPECIES)
	{
		if (!taxon.parent_taxon) return "";
		return firstUppercaseOnly(taxon.parent_taxon->name) + " " + bin_name;
	}
	else if (taxon.level == FBTaxonLevel::SUBSPECIES)
	{
		if (!taxon.parent_taxon || !taxon.parent_taxon->parent_taxon) return "";
		return firstUppercaseOnly(taxon.parent_taxon->parent_taxon->name) + " " + lowercase(taxon.parent_taxon->name) + " " + bin_name;
	}
	else return "";
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

		queue<FBTaxon*> taxa_to_check;
		taxa_to_check.push(top_level);
		while (!taxa_to_check.empty())
		{
			FBTaxon* checking = taxa_to_check.front();
			taxa_to_check.pop();
			for (FBTaxon* sub : checking->sub_taxa)
			{
				if (sub->level == FBTaxonLevel::SPECIES)
				{
					fungi_index.emplace(sub->parent_taxon->name + " " + sub->name, (FBFungus*)sub);
				}
				else if (sub->level == FBTaxonLevel::SUBSPECIES)
				{
					fungi_index.emplace(sub->parent_taxon->parent_taxon->name + " " + sub->parent_taxon->name + " " + sub->name, (FBFungus*)sub);
				}
				taxa_to_check.push(sub);
			}
		}

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

set<FBFungus*> FBDatabase::search(bool(*evaluator)(const FBFungus&))
{
	set<FBFungus*> matched_fungi;
	if (!is_open) return matched_fungi;

	for (pair<string, FBFungus*> fungus : fungi_index)
	{
		if (evaluator(*fungus.second)) matched_fungi.insert(fungus.second);
	}

	return matched_fungi;
}

FBFungus* FBDatabase::findBinomial(string binomial_name)
{
	if (!is_open) return NULL;
	return fungi_index[binomial_name];
}

FBTaxon* FBDatabase::find(string name, FBTaxonLevel level)
{
	if (!is_open) return NULL;

	queue<FBTaxon*> taxa_to_check;
	taxa_to_check.push(top_level);
	while (!taxa_to_check.empty())
	{
		FBTaxon* checking = taxa_to_check.front();
		taxa_to_check.pop();
		if (checking->level == level)
		{
			if (checking->name == name) return checking;
		}
		else if (checking->level < level)
		{
			for (FBTaxon* child : checking->sub_taxa) taxa_to_check.push(child);
		}
	}
	return NULL;
}

set<FBTaxon*> FBDatabase::list(FBTaxon* parent, FBTaxonLevel of_level)
{
	set<FBTaxon*> matched;

	if (!is_open) return matched;
	if (!parent || !checkIsInHeirarchy(parent)) return matched;

	queue<FBTaxon*> taxa_to_check;
	taxa_to_check.push(parent);
	while (!taxa_to_check.empty())
	{
		FBTaxon* checking = taxa_to_check.front();
		taxa_to_check.pop();
		for (FBTaxon* sub : checking->sub_taxa)
		{
			if (sub->level == of_level)
				matched.insert(sub);
			else
				taxa_to_check.push(sub);
		}
	}

	return matched;
}

bool FBDatabase::insert(const FBTaxon& new_taxon, FBTaxon* parent)
{
	if (!is_open) return false;
	if (!checkIsInHeirarchy(parent)) return false;
	if (new_taxon.level <= parent->level) return false;
	
	FBTaxon* child = new FBTaxon(new_taxon);
	parent->sub_taxa.insert(child);
	child->parent_taxon = parent;

	string binomial_name = getBinomialName(*child);
	if (findBinomial(binomial_name))
	{
		parent->sub_taxa.erase(child);
		delete child;
		return false;
	}

	if (child->level == FBTaxonLevel::SPECIES || child->level == FBTaxonLevel::SUBSPECIES)
	{
		delete child;
		parent->sub_taxa.erase(child);
		child = new FBFungus();
		*child = new_taxon;
		parent->sub_taxa.insert(child);
		child->parent_taxon = parent;

		fungi_index.emplace(getBinomialName(*child), (FBFungus*)child);
	}

	return true;
}

bool FBDatabase::remove(FBTaxon* taxon)
{
	if (!is_open) return false;
	if (!checkIsInHeirarchy(taxon)) return false;

	taxon->parent_taxon->sub_taxa.erase(taxon);

	if (taxon->level == FBTaxonLevel::SPECIES || taxon->level == FBTaxonLevel::SUBSPECIES)
		fungi_index.erase(getBinomialName(*taxon));
	
	delete taxon;

	return true;
}

bool FBDatabase::flush()
{
	if (top_level == NULL || !is_open) return false;

	ifstream file_backup;
	file_backup.open(file_address);
	if (!file_backup.is_open()) return false;
	stringstream buffer;
	buffer << file_backup.rdbuf();
	file_backup.close();

	ofstream file;
	file.open(file_address);
	if (!file.is_open()) return false;
	
	try
	{
		file << jsonifyObject(serialise(*top_level));
	}
	catch (exception e)
	{
		file << buffer.str();
	}

	file.close();

	return true;
}

bool FBDatabase::isOpen()
{
	return is_open;
}

FBDatabase::~FBDatabase()
{
	if (is_open && top_level) flush();
	unloadDatabase();
	is_open = false;
}
