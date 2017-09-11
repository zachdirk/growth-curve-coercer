#include <string>
#include <unordered_map>
#include <sstream>
#include "Polygon.h"
#include "Tools.h"
#include "SpeciesSubstitution.h"

bool subPolygon(Polygon &p, const std::vector<substitutionMap> &subMaps){
    std::string polySpecies = p.getSpecies();
    std::string polyBec = p.getBEC();
    for (auto &subMap: subMaps){
        std::string s = subMap.species;
        std::string b = subMap.bec;
        if (caseInsensitiveStringCompare(s, polySpecies) && caseInsensitiveStringCompare(b, polyBec)){
            p.setSpecies(subMap.speciesSubstitution);
            return(true);
        }
    }
    return(false);
}

std::vector<substitutionMap> makeSubMapFromCSV(const Table &subTable){
    std::unordered_map<std::string, int> indices = parseHeader(subTable);
    int speciesIndex;
    if (indices.find("species") != indices.end())
        speciesIndex = indices["species"];
    else if (indices.find("ldspp") != indices.end())
        speciesIndex = indices["ldspp"];
    else {
        std::stringstream err;
        err << "Error! No species index in substitution table." << std::endl << getTime() 
            << "Please ensure your table has a header, and that one field is labeled \"species\".";
        throw err.str();
    }
    int subSpeciesIndex;
    if (indices.find("substitutespecies") != indices.end())
        subSpeciesIndex = indices["substitutespecies"];
    else if (indices.find("sub") != indices.end())
        subSpeciesIndex = indices["sub"];
    else {
        std::stringstream err;
        err << "Error! No substitutespecies index in substitution table." << std::endl << getTime()  
            << "Please ensure your table has a header, and that one field is labeled \"substitutespecies\".";
        throw err.str();
    }
    int becIndex;
    if (indices.find("bec") != indices.end())
        becIndex = indices["bec"];
    else {
        std::stringstream err;
        err << "Error! No bec index in substitution table." << std::endl << getTime()  
            << "Please ensure your table has a header, and that one field is labeled \"bec\".";
        throw err.str();
    }

    std::vector<substitutionMap> subMaps;
    for (int i = 1; i < subTable.size(); i++){
        Row r = subTable[i];
        std::string species = r[speciesIndex];
        std::string subSpecies = r[subSpeciesIndex];
        std::string bec = r[becIndex];
        substitutionMap sub = {species,subSpecies,bec};
        subMaps.push_back(sub);
    }
    return(subMaps);
}
