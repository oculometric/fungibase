// FungiBase.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>

#include "serialise.h"

int main()
{
    cout << "Hello World!\n";

    FBJsonObject sub_object;
    sub_object["first item"] = FBJsonElement{ FBJsonElementType::INT, -111 };
    sub_object["second item"] = FBJsonElement{ FBJsonElementType::INT, 111 };
    sub_object["third item"] = FBJsonElement{ FBJsonElementType::STRING, 1, "sub" };

    FBJsonObject object;

    object["entry"] = FBJsonElement{ FBJsonElementType::INT, 201 };
    object["my string"] = FBJsonElement{ FBJsonElementType::STRING, 0, "waa waa waa" };
    object["sub object"] = FBJsonElement{ FBJsonElementType::OBJECT, 0, "", sub_object };

    FBJsonElement array_element;
    array_element.type = FBJsonElementType::ARRAY;
    array_element.array_value.push_back(FBJsonElement{ FBJsonElementType::INT, 1 });
    array_element.array_value.push_back(FBJsonElement{ FBJsonElementType::INT, 2 });
    array_element.array_value.push_back(FBJsonElement{ FBJsonElementType::STRING, 0, "3" });

    object["array"] = array_element;

    cout << jsonifyObject(object) << endl;

    cout << endl << endl;

    FBTaxon kingdom = FBTaxon{ "Fungi", FBTaxonLevel::KINGDOM, "Contains all fungi." };
    FBTaxon basidiomycota = FBTaxon{ "Basidiomycota", FBTaxonLevel::PHYLUM, "Filamentous fungi composed of hyphae (mostly)." };
    kingdom.sub_taxa.push_back(&basidiomycota);
    basidiomycota.parent_taxa = &kingdom;
    FBTaxon agaricomycetes = FBTaxon{ "Agaricomycetes", FBTaxonLevel::CLASS, "" };
    basidiomycota.sub_taxa.push_back(&agaricomycetes);
    agaricomycetes.parent_taxa = &basidiomycota;
    FBTaxon agaricales = FBTaxon{ "Agaricales", FBTaxonLevel::ORDER, "" };
    agaricomycetes.sub_taxa.push_back(&agaricales);
    agaricales.parent_taxa = &agaricomycetes;
    FBTaxon amanitaceae = FBTaxon{ "Amanitaceae", FBTaxonLevel::FAMILY, "" };
    agaricales.sub_taxa.push_back(&amanitaceae);
    amanitaceae.parent_taxa = &agaricales;
    FBTaxon amanita = FBTaxon{ "Amanita", FBTaxonLevel::GENUS, "" };
    amanitaceae.sub_taxa.push_back(&amanita);
    amanita.parent_taxa = &amanitaceae;
    FBFungus muscaria;
    muscaria.feeding_type = FBFeedingType::SAPROBIC;
    muscaria.name = "muscaria";
    muscaria.level = FBTaxonLevel::SPECIES;
    muscaria.description = "Commonly known as the fly agaric.";
    amanita.sub_taxa.push_back(&muscaria);
    muscaria.parent_taxa = &amanita;

    string json = jsonifyObject(serialise(kingdom));
    FBJsonObject dejsonified = objectifyJson(json);
    string rejson = jsonifyObject(dejsonified);

    FBTaxon test;
    deserialise(dejsonified, test);
    string rerejson = jsonifyObject(serialise(test));

    cout << json << endl;
    cout << endl;
    cout << rejson << endl;
    cout << endl;
    cout << rerejson << endl;

    ofstream file;
    file.open("json.json");

    file << json << endl << endl;
    file << rejson << endl << endl;
    file << rerejson << endl << endl;

    file.close();
}
