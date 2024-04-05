#include "serialise.h"

string jsonifyElement(const FBJsonElement& element)
{
	string str;

	switch (element.type)
	{
	case FBJsonElementType::INT:
		str += to_string(element.int_value);
		break;
	case FBJsonElementType::STRING:
		str += '\"';
		str += element.string_value;
		str += '\"';
		break;
	case FBJsonElementType::OBJECT:
		str += jsonifyObject(element.object_value);
		break;
	case FBJsonElementType::ARRAY:
		str += "[ ";
		bool is_first_element = true;
		for (FBJsonElement sub_element : element.array_value)
		{
			if (is_first_element) is_first_element = false;
			else str += ", ";
			str += jsonifyElement(sub_element);
		}
		str += " ]";
		break;
	}

	return str;
}

string jsonifyObject(const FBJsonObject& object)
{
	string str = "{ ";

	bool is_first_element = true;
	for (pair<string, FBJsonElement> p : object)
	{
		if (is_first_element) is_first_element = false;
		else str += ", ";

		str += '\"';
		str += p.first;
		str += "\" : ";
		
		str += jsonifyElement(p.second);
	}

	str += " }";
	return str;
}

string substringBetweenMatching(const string& str, char a, char b)
{
	if (a == b) return "";
	if (str.length() == 0) return "";
	int start = 0;
	int end = 0;
	int depth = 0;
	for (int i = 0; i < str.length(); i++)
	{
		if (str[i] == a && depth == 0) start = i;
		if (str[i] == a) depth++;
		if (str[i] == b) depth--;
		if (str[i] == b && depth == 0)
		{
			end = i;
			return str.substr(start + 1, (end - start) - 1);
		}
	}

	return "";
}

string removeNonEnclosedWhitespace(const string& str)
{
	string out;
	out.reserve(str.length());
	
	bool inside_quotes = false;

	for (char c : str)
	{
		if (c == '\"') inside_quotes = !inside_quotes;
		if (!inside_quotes && (c == ' ' || c == '\n' || c == '\t' || c == '\v' || c == '\f' || c == '\r')) continue;
		out += c;
	}

	return out;
}

vector<string> bracketAwareSplit(const string& str)
{
	vector<string> list;
	list.push_back("");
	
	int sub_depth = 0;
	bool in_quotes = false;
	for (char c : str)
	{
		if (c == ',' && sub_depth == 0 && !in_quotes)
		{
			list.push_back("");
			continue;
		}

		list[list.size() - 1].push_back(c);

		if (c == '}' || c == ']') sub_depth--;
		if (c == '{' || c == '[') sub_depth++;
		if (c == '\"') in_quotes = !in_quotes;
	}

	return list;
}

FBJsonObject objectifyJsonInner(const string&);

FBJsonElement elementifyJson(const string& str)
{
	FBJsonElement payload;
	if (str[0] == '"')
	{
		payload.type = FBJsonElementType::STRING;
		payload.string_value = str.substr(1, str.length() - 2);
	}
	else if (str[0] == '{')
	{
		payload.type = FBJsonElementType::OBJECT;
		payload.object_value = objectifyJsonInner(str);
	}
	else if (str[0] == '[')
	{
		payload.type = FBJsonElementType::ARRAY;
		vector<string> sub_elements = bracketAwareSplit(str.substr(1, str.length() - 2));
		for (string sub_el : sub_elements)
		{
			if (sub_el.empty()) continue;
			payload.array_value.push_back(elementifyJson(sub_el));
		}
	}
	else
	{
		payload.type = FBJsonElementType::INT;
		payload.int_value = stoi(str);
	}

	return payload;
}

FBJsonObject objectifyJsonInner(const string& str)
{
	FBJsonObject object;

	string substr = substringBetweenMatching(str, '{', '}');
	vector<string> elements = bracketAwareSplit(substr);

	for (string str_el : elements)
	{
		if (str_el.empty()) continue;

		string el_name = str_el.substr(1, str_el.find('"', 1) - 1);
		string el_payload = str_el.substr(str_el.find('"', 1) + 2);
		
		if (el_name.empty() || el_payload.empty()) continue;

		object[el_name] = elementifyJson(el_payload);
	}

	return object;
}

FBJsonObject objectifyJson(const string& str)
{
	//if (!checkJsonIntegrity(str)) return FBJsonObject();
	return objectifyJsonInner(removeNonEnclosedWhitespace(str));
}

FBJsonElement makeIntElement(int i)
{
	FBJsonElement e;
	e.type = FBJsonElementType::INT;
	e.int_value = i;

	return e;
}

FBJsonElement makeStringElement(string s)
{
	FBJsonElement e;
	e.type = FBJsonElementType::STRING;
	e.string_value = s;

	return e;
}

FBJsonElement makeObjectElement(FBJsonObject o)
{
	FBJsonElement e;
	e.type = FBJsonElementType::OBJECT;
	e.object_value = o;

	return e;
}

FBJsonElement makeArrayElement(vector<FBJsonElement> a)
{
	FBJsonElement e;
	e.type = FBJsonElementType::ARRAY;
	e.array_value = a;

	return e;
}

int getIntElement(FBJsonElement e)
{
	if (e.type != FBJsonElementType::INT) return 0;
	return e.int_value;
}

string getStringElement(FBJsonElement e)
{
	if (e.type != FBJsonElementType::STRING) return "";
	return e.string_value;
}

FBJsonObject getObjectElement(FBJsonElement e)
{
	if (e.type != FBJsonElementType::OBJECT) return FBJsonObject();
	return e.object_value;
}

vector<FBJsonElement> getArrayElement(FBJsonElement e)
{
	if (e.type != FBJsonElementType::ARRAY) return vector<FBJsonElement>();
	return e.array_value;
}

FBJsonObject serialise(const FBTaxon& taxon)
{
	FBJsonObject taxon_object;

	taxon_object["name"] = makeStringElement(taxon.name);
	taxon_object["level"] = makeIntElement(taxon.level);
	taxon_object["description"] = makeStringElement(taxon.description);
	taxon_object["sub_taxa"] = FBJsonElement{ FBJsonElementType::ARRAY };
	for (FBTaxon* sub_taxon : taxon.sub_taxa)
	{
		if (sub_taxon->level == FBTaxonLevel::SPECIES || sub_taxon->level == FBTaxonLevel::SUBSPECIES)
			taxon_object["sub_taxa"].array_value.push_back(makeObjectElement(serialise(*(FBFungus*)sub_taxon)));
		else
			taxon_object["sub_taxa"].array_value.push_back(makeObjectElement(serialise(*sub_taxon)));
	}
	taxon_object["sources"] = FBJsonElement{ FBJsonElementType::ARRAY };
	for (FBSource h : taxon.sources) taxon_object["sources"].array_value.push_back(makeObjectElement(serialise(h)));

	return taxon_object;
}

void deserialise(FBJsonObject& taxon_object, FBTaxon& taxon)
{
	taxon.name = getStringElement(taxon_object["name"]);
	taxon.level = (FBTaxonLevel)getIntElement(taxon_object["level"]);
	taxon.description = getStringElement(taxon_object["description"]);
	for (FBJsonElement sub_taxon_el : getArrayElement(taxon_object["sub_taxa"]))
	{
		FBTaxon* sub_taxon = new FBTaxon();
		FBJsonObject obj = getObjectElement(sub_taxon_el);
		deserialise(obj, *sub_taxon);
		if (sub_taxon->level == FBTaxonLevel::SPECIES || sub_taxon->level == FBTaxonLevel::SUBSPECIES)
		{
			delete sub_taxon;
			sub_taxon = new FBFungus();
			deserialise(obj, *((FBFungus*)sub_taxon));
		}
		sub_taxon->parent_taxon = &taxon;
		taxon.sub_taxa.insert(sub_taxon);
	}
	for (FBJsonElement source_el : getArrayElement(taxon_object["sources"]))
	{
		FBSource s;
		FBJsonObject obj = getObjectElement(source_el);
		deserialise(obj, s);
		taxon.sources.insert(s);
	}
}

FBJsonObject serialise(const FBSizeRange& sr)
{
	FBJsonObject sr_object;

	// TODO:

	return sr_object;
}

void deserialise(FBJsonObject& sr_object, FBSizeRange& sr)
{
	// TODO:
}

FBJsonObject serialise(const FBFruitingBodyDetails& fbd)
{
	FBJsonObject fbd_object;

	fbd_object["type"] = makeIntElement(fbd.type);
	fbd_object["has_distinct_cap"] = makeIntElement(fbd.has_distinct_cap);
	fbd_object["cap_shape"] = makeIntElement(fbd.cap_shape);
	fbd_object["cap_diameter"] = makeObjectElement(serialise(fbd.cap_diameter));
	fbd_object["cap_colour"] = makeIntElement(fbd.cap_colour);
	fbd_object["cap_description"] = makeStringElement(fbd.cap_description);
	fbd_object["stipe_colour"] = makeIntElement(fbd.stipe_colour);
	fbd_object["stipe_description"] = makeStringElement(fbd.stipe_description);

	return fbd_object;
}

void deserialise(FBJsonObject& fbd_object, FBFruitingBodyDetails& fbd)
{
	fbd.type = (FBFruitingBodyType)getIntElement(fbd_object["type"]);
	fbd.has_distinct_cap = getIntElement(fbd_object["has_distinct_cap"]);
	fbd.cap_shape = (FBCapShape)getIntElement(fbd_object["cap_shape"]);
	FBJsonObject obj = getObjectElement(fbd_object["cap_diameter"]);
	deserialise(obj, fbd.cap_diameter);
	fbd.cap_colour = (FBColour)getIntElement(fbd_object["cap_colour"]);
	fbd.cap_description = getStringElement(fbd_object["cap_description"]);
	fbd.stipe_colour = (FBColour)getIntElement(fbd_object["stipe_colour"]);
	fbd.stipe_description = getStringElement(fbd_object["stipe_description"]);
}

FBJsonObject serialise(const FBSporeSurfaceDetails& ssd)
{
	FBJsonObject ssd_object;

	ssd_object["type"] = makeIntElement(ssd.type);
	ssd_object["gill_attachment"] = makeIntElement(ssd.gill_attachment);
	ssd_object["gill_shape"] = makeIntElement(ssd.gill_shape);

	return ssd_object;
}

void deserialise(FBJsonObject& ssd_object, FBSporeSurfaceDetails& ssd)
{
	ssd.type = (FBSporeSurfaceType)getIntElement(ssd_object["type"]);
	ssd.gill_attachment = (FBGillAttachmentType)getIntElement(ssd_object["gill_attachment"]);
	ssd.gill_shape = (FBGillShapeType)getIntElement(ssd_object["gill_shape"]);
}

FBJsonObject serialise(const FBReproductiveDetails& rd)
{
	FBJsonObject rd_object;
	
	// TODO:

	return rd_object;
}

void deserialise(FBJsonObject& rd_object, FBReproductiveDetails& rd)
{
	// TODO:
}

FBJsonObject serialise(const FBSporeDetails& sd)
{
	FBJsonObject sd_object;

	sd_object["has_spores"] = makeIntElement(sd.has_spores);
	sd_object["spore_shape"] = makeIntElement(sd.spore_shape);
	sd_object["spore_size_range"] = makeObjectElement(serialise(sd.spore_size_range));
	sd_object["spore_print_colour"] = makeIntElement(sd.spore_print_colour);

	return sd_object;
}

void deserialise(FBJsonObject& sd_object, FBSporeDetails& sd)
{
	sd.has_spores = getIntElement(sd_object["has_spores"]);
	sd.spore_shape = (FBSporeShape)getIntElement(sd_object["spore_shape"]);
	FBJsonObject obj = getObjectElement(sd_object["spore_size_range"]);
	deserialise(obj, sd.spore_size_range);
	sd.spore_print_colour = (FBColour)getIntElement(sd_object["spore_print_colour"]);
}

FBJsonObject serialise(const FBDistribution& d)
{
	FBJsonObject d_object;

	// TODO:

	return d_object;
}

void deserialise(FBJsonObject& d_object, FBDistribution& d)
{
	// TODO:
}

FBJsonObject serialise(const FBFungus& fungus)
{
	FBJsonObject fungus_object = serialise(*((FBTaxon*)(&fungus)));

	fungus_object["fruiting_body_details"] = makeObjectElement(serialise(fungus.fruiting_body_details));
	fungus_object["spore_surface_details"] = makeObjectElement(serialise(fungus.spore_surface_details));
	fungus_object["reproductive_details"] = makeObjectElement(serialise(fungus.reproductive_details));
	fungus_object["spore_details"] = makeObjectElement(serialise(fungus.spore_details));
	fungus_object["distribution"] = makeObjectElement(serialise(fungus.distribution));
	fungus_object["habitat_type"] = FBJsonElement{ FBJsonElementType::ARRAY };
	for (FBHabitatType h : fungus.habitat_type) fungus_object["habitat_type"].array_value.push_back(makeIntElement(h));
	fungus_object["feeding_type"] = makeIntElement(fungus.feeding_type);
	fungus_object["tags"] = FBJsonElement{ FBJsonElementType::ARRAY };
	for (FBTag h : fungus.tags) fungus_object["tags"].array_value.push_back(makeIntElement(h));

	return fungus_object;
}

void deserialise(FBJsonObject& fungus_object, FBFungus& fungus)
{
	deserialise(fungus_object, *((FBTaxon*)(&fungus)));

	FBJsonObject object = getObjectElement(fungus_object["fruiting_body_details"]);
	deserialise(object, fungus.fruiting_body_details);
	object = getObjectElement(fungus_object["spore_surface_details"]);
	deserialise(object, fungus.spore_surface_details);
	object = getObjectElement(fungus_object["reproductive_details"]);
	deserialise(object, fungus.reproductive_details);
	object = getObjectElement(fungus_object["spore_details"]);
	deserialise(object, fungus.spore_details);
	object = getObjectElement(fungus_object["distribution"]);
	deserialise(object, fungus.distribution);
	for (FBJsonElement el : getArrayElement(fungus_object["habitat_type"]))
	{
		fungus.habitat_type.insert((FBHabitatType)getIntElement(el));
	}
	fungus.feeding_type = (FBFeedingType)getIntElement(fungus_object["feeding_type"]);
	for (FBJsonElement el : getArrayElement(fungus_object["tags"]))
	{
		fungus.tags.insert((FBTag)getIntElement(el));
	}
}

FBJsonObject serialise(const FBSource& source)
{
	FBJsonObject source_object;

	source_object["index"] = makeIntElement(source.index);
	source_object["link"] = makeStringElement(source.link);
	source_object["description"] = makeStringElement(source.description);
	source_object["date_accessed"] = makeStringElement(source.date_accessed);

	return source_object;
}

void deserialise(FBJsonObject& source_object, FBSource& source)
{
	source.index = getIntElement(source_object["index"]);
	source.link = getStringElement(source_object["link"]);
	source.description = getStringElement(source_object["description"]);
	source.date_accessed = getStringElement(source_object["date_accessed"]);
}