#ifndef SPECIESSUBSTITUTION_H
#define SPECIESSUBSTITUTION_H

/*
 * SpeciesSubstitution handles the substituting of species that don't have growth curves.
 */

#include <unordered_map>
#include <string>
#include "Polygon.h"
#include "Tools.h"

struct substitutionMap {
    std::string species;
    std::string speciesSubstitution;
    std::string bec;
};

bool subPolygon(Polygon &p, const std::vector<substitutionMap> &subMaps);
std::vector<substitutionMap> makeSubMapFromCSV(const Table &subTable);

#endif