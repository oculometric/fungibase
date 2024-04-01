#include "taxonomy.h"
#include "fungi.h"

string getDescription(FBTaxonLevel taxon_level)
{
	switch (taxon_level)
	{
	case FBTaxonLevel::KINGDOM: return "Kingdom";
	case FBTaxonLevel::PHYLUM: return "Phylum";
	case FBTaxonLevel::SUBPHYLUM: return "Subphylum";
	case FBTaxonLevel::CLASS: return "Class";
	case FBTaxonLevel::SUBCLASS: return "Subclass";
	case FBTaxonLevel::ORDER: return "Order";
	case FBTaxonLevel::FAMILY: return "Family";
	case FBTaxonLevel::GENUS: return "Genus";
	case FBTaxonLevel::SPECIES: return "Species";
	case FBTaxonLevel::SUBSPECIES: return "Subspecies";
	default: return "Unknown taxon";
	}
}

string getDescription(FBFruitingBodyType fruiting_body_type)
{
	switch (fruiting_body_type)
	{
	default: return "Unknown fruiting body type";
	}
}

string getDescription(FBCapShape cap_shape)
{
	switch (cap_shape)
	{
	default: return "Unknown cap shape";
	}
}

string getDescription(FBColour colour)
{
	switch (colour)
	{
	default: return "Unknown colour";
	}
}

string getDescription(FBSporeSurfaceType spore_surface_type)
{
	switch (spore_surface_type)
	{
	case FBSporeSurfaceType::GILLS: return "Gills";
	case FBSporeSurfaceType::PORES: return "Pores";
	case FBSporeSurfaceType::TEETH: return "Teeth";
	case FBSporeSurfaceType::SAC: return "Sac";
	case FBSporeSurfaceType::NONE: return "None";
	default: return "Unknown spore surface type";
	}
}

string getDescription(FBGillAttachmentType gill_attachment_type)
{
	switch (gill_attachment_type)
	{
	default: return "Unknown gill attachment type";
	}
}

string getDescription(FBGillShapeType gill_shape_type)
{
	switch (gill_shape_type)
	{
	default: return "Unknown gill shape type";
	}
}

string getDescription(FBFeedingType feeding_type)
{
	switch (feeding_type)
	{
	case FBFeedingType::SAPROBIC: return "Saprobic";
	case FBFeedingType::PARASITIC: return "Parasitic";
	default: return "Unknown feeding type";
	}
}

string getDescription(FBSporeShape spore_shape)
{
	switch (spore_shape)
	{
	default: return "Unknown spore shape";
	}
}

string getDescription(FBHabitatType habitat_type)
{
	switch (habitat_type)
	{
	default: return "Unknown habitat type";
	}
}

string getDescription(FBTag tag)
{
	switch (tag)
	{
	case FBTag::HAS_CAP: return "Has cap";
	case FBTag::LACKS_CAP: return "Lacks cap";

	case FBTag::MACROSCOPIC: return "Macroscopic";
	case FBTag::MICROSCOPIC: return "Microscopic";

	case FBTag::HAS_BASIDIA: return "Has basidia";
	case FBTag::HAS_ASCI: return "Has asci";

	case FBTag::HAS_GILLS: return "Has gills";
	case FBTag::HAS_PORES: return "Has pores";
	case FBTag::HAS_TEETH: return "Has teeth";
	case FBTag::HAS_SAC: return "Has sac";

	case FBTag::FEEDS_SAPROBICALLY: return "Feeds saprobically";
	case FBTag::FEEDS_PARASITICALLY: return "Feeds parasitically";

	default: return "Unknown tag";
	}
}