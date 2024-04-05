#pragma once

#include <string>
#include <vector>
#include <map>

using namespace std;

struct FBJsonElement;

typedef map<string, FBJsonElement> FBJsonObject;

enum FBJsonElementType
{
	INT,
	STRING,
	OBJECT,
	ARRAY,
	INVALID
};

struct FBJsonElement
{
	FBJsonElementType type = (FBJsonElementType)(-1);

	int int_value = 0;
	string string_value = "";
	FBJsonObject object_value;
	vector<FBJsonElement> array_value;
};

string jsonifyObject(const FBJsonObject&);
FBJsonObject objectifyJson(const string&);

FBJsonElement makeIntElement(int);
FBJsonElement makeStringElement(string);
FBJsonElement makeObjectElement(FBJsonObject);
FBJsonElement makeArrayElement(vector<FBJsonElement>);

int getIntElement(FBJsonElement);
string getStringElement(FBJsonElement);
FBJsonObject getObjectElement(FBJsonElement);
vector<FBJsonElement> getArrayElement(FBJsonElement);

#include "taxonomy.h"

FBJsonObject serialise(const FBTaxon&);
void deserialise(FBJsonObject&, FBTaxon&);

#include "fungi.h"

FBJsonObject serialise(const FBSizeRange&);
void deserialise(FBJsonObject&, FBSizeRange&);

FBJsonObject serialise(const FBFruitingBodyDetails&);
void deserialise(FBJsonObject&, FBFruitingBodyDetails&);

FBJsonObject serialise(const FBSporeSurfaceDetails&);
void deserialise(FBJsonObject&, FBSporeSurfaceDetails&);

FBJsonObject serialise(const FBReproductiveDetails&);
void deserialise(FBJsonObject&, FBReproductiveDetails&);

FBJsonObject serialise(const FBSporeDetails&);
void deserialise(FBJsonObject&, FBSporeDetails&);

FBJsonObject serialise(const FBDistribution&);
void deserialise(FBJsonObject&, FBDistribution&);

FBJsonObject serialise(const FBFungus&);
void deserialise(FBJsonObject&, FBFungus&);

FBJsonObject serialise(const FBSource&);
void deserialise(FBJsonObject&, FBSource&);