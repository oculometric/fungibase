// FungiBase.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>

#include "serialise.h"
#include "database.h"

int main()
{
    cout << "Hello World!\n";

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

    cout << json << endl;

    ofstream file;
    file.open("backup.json");
    file << json << endl << endl;
    file.close();

    FBDatabase database("database.json");
    cout << database.reload() << endl;
    cout << database.flush() << endl;
}
