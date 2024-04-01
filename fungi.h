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
	FBFruitingBodyType type = (FBFruitingBodyType)(-1);
	bool has_distinct_cap = false;
	FBCapShape cap_shape = (FBCapShape)(-1);
	FBSizeRange cap_diameter;
	FBColour cap_colour = (FBColour)(-1);
	string cap_description = "";
	FBColour stipe_colour = (FBColour)(-1);
	string stipe_description = "";
};

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
	FBSporeSurfaceType type = (FBSporeSurfaceType)(-1);
	FBGillAttachmentType gill_attachment = (FBGillAttachmentType)(-1);
	FBGillShapeType gill_shape = (FBGillShapeType)(-1);
};

enum FBFeedingType
{
	SAPROBIC,
	PARASITIC
	// TODO:
};

string getDescription(FBFeedingType);

struct FBReproductiveDetails
{
	// TODO:
};

enum FBSporeShape
{
	// TODO:
};

string getDescription(FBSporeShape);

struct FBSporeDetails
{
	bool has_spores = false;
	FBSporeShape spore_shape = (FBSporeShape)(-1);
	FBSizeRange spore_size_range;
	FBColour spore_print_colour = (FBColour)(-1);
};

struct FBDistribution
{
	// TODO:
};

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

	FEEDS_SAPROBICALLY,
	FEEDS_PARASITICALLY
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
	FBFeedingType feeding_type = (FBFeedingType)(-1);

	vector<FBTag> tags;
};