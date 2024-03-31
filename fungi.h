#pragma once

#include "taxonomy.h"

enum FBFruitingBodyType
{
	// TODO:
};

string getDescription(FBFruitingBodyType);

enum FBCapShape
{
	// TODO:
};

string getDescription(FBCapShape);

struct FBSizeRange
{
	// TODO:
};

enum FBColour
{
	// TODO:
};

string getDescription(FBColour);

struct FBFruitingBodyDetails
{
	FBFruitingBodyType type;
	bool has_distinct_cap;
	FBCapShape cap_shape;
	FBSizeRange cap_diameter;
	FBColour cap_colour;
	string cap_description;
	FBColour stipe_colour;
	string stipe_description;
};

string serialise(const FBFruitingBodyDetails&);
void deserialise(const string&, FBFruitingBodyDetails&);

enum FBSporeSurfaceType
{
	GILLS,
	PORES,
	TEETH,
	SAC,
	NONE
	// TODO: expand
};

string getDescription(FBSporeSurfaceType);

enum FBGillAttachmentType
{
	// TODO:
};

string getDescription(FBGillAttachmentType);

enum FBGillShapeType
{
	// TODO:
};

string getDescription(FBGillShapeType);

struct FBSporeSurfaceDetails
{
	FBSporeSurfaceType type;
	FBGillAttachmentType gill_attachment;
	FBGillShapeType gill_shape;
};

string serialise(const FBSporeSurfaceDetails&);
void deserialise(const string&, FBSporeSurfaceDetails&);

enum FBFeedingType
{
	SAPROBIC,
	PARASITIC,
	NONE
	// TODO:
};

string getDescription(FBFeedingType);

struct FBReproductiveDetails
{
	// TODO:
};

string serialise(const FBReproductiveDetails&);
void deserialise(const string&, FBReproductiveDetails&);

enum FBSporeShape
{
	// TODO:
};

string getDescription(FBSporeShape);

struct FBSporeDetails
{
	bool has_spores;
	FBSporeShape spore_shape;
	FBSizeRange spore_size_range;
	FBColour spore_print_colour;
};

string serialise(const FBSporeDetails&);
void deserialise(const string&, FBSporeDetails&);

struct FBDistribution
{
	// TODO:
};

string serialise(const FBDistribution&);
void deserialise(const string&, FBDistribution&);

enum FBHabitatType
{
	// TODO:
};

string getDescription(FBHabitatType);

enum FBTag
{
	// TODO:
	HAS_CAP,
	LACKS_CAP,

	MACROSCOPIC,
	MICROSCOPIC,

	HAS_BASIDIA,
	HAS_ASCI,

	HAS_GILLS,
	HAS_PORES,
	HAS_TEETH,
	HAS_SAC,

	SAPROBIC,
	PARASITIC
};

string getDescription(FBTag);

struct FBFungus : public FBTaxon
{
	FBFruitingBodyDetails fruiting_body_details;
	FBSporeSurfaceDetails spore_surface_details;
	FBReproductiveDetails reproductive_details;
	FBSporeDetails spore_details;
	FBDistribution distribution;
	vector<FBHabitatType> habitat_type;
	FBFeedingType feeding_type;

	vector<FBTag> tags;
};

string serialise(const FBFungus&);
void deserialise(const string&, FBFungus&);