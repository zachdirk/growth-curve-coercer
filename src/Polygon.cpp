#include <iostream>
#include <string>
#include <locale>
#include <vector>
#include <sstream>
#include <fstream>
#include "Polygon.h"
#include "Tools.h"

Polygon::Polygon(int ID, double SiteIndex, std::string BEC, std::string Species, int AU){
    mID = ID;
    mSiteIndex = SiteIndex;
    mBEC = BEC;
    mSpecies = Species;
    mAU = AU;
}

std::ostream& operator<< (std::ostream &out, const Polygon &p){
    out << "This Polygon has:\n";
    out << "ID: " << p.mID << '\n';
    out << "Site Index: " << p.mSiteIndex << '\n';
    out << "BEC: " << p.mBEC << '\n';
    out << "Species: " << p.mSpecies << '\n';
    out << "AU: " << p.mAU << '\n';
    return out;
}

bool Polygon::isMatch(const GrowthCurve &gc) {
    if (mSiteIndex < gc.getSI_MIN() || mSiteIndex >= gc.getSI_MAX()){
        return(false);
    }
    if (!caseInsensitiveStringCompare(mBEC, gc.getBEC()) || !caseInsensitiveStringCompare(mSpecies, gc.getSpecies())){
        return(false);
    }
    
    
    return(true);
}

bool matchingSpeciesBEC(const GrowthCurve &gc, const Polygon &p){
    return(caseInsensitiveStringCompare(p.getSpecies(), gc.getSpecies()) && caseInsensitiveStringCompare(p.getBEC(), gc.getBEC()));
}

bool match(Polygon &p, const std::vector<GrowthCurve> &curves){
    for (auto &gc: curves){
        if (p.isMatch(gc)){
            p.setAU(gc.getAU());
            return(true);
        }
    }
    return(false);
}

std::vector<Polygon> makePolygonsFromCSV(const Table &polygonTable){
    std::unordered_map<std::string, int> indices = parseHeader(polygonTable);
    int IDIndex, siteIndexIndex, BECIndex, speciesIndex;
    try{
        IDIndex = findMapIndex(indices, "id");
        siteIndexIndex = findMapIndex(indices, "siteindex");
        BECIndex = findMapIndex(indices, "bec");
        speciesIndex = findMapIndex(indices, "ldspp");
    } catch(std::string index) {
        std::stringstream err;
        err << "Error! No " << index << " in polygon input table." << std::endl 
            << "Please ensure your table has a header, and that one field is labeled \"" << index <<"\".";
        throw err.str();
    }
    std::vector<Polygon> polygons;
    polygons.reserve(polygonTable.size());
    for (int i = 1; i < polygonTable.size(); i++){
        Row r = polygonTable[i];
        int ID = std::stoi(r[IDIndex]);
        double SI = -1; //-1 is an invalid site index so there's no chance of this being confusing
        if (!r[siteIndexIndex].empty()) //if the site index is empty and we try to stod it, it would crash
            SI = std::stod(r[siteIndexIndex]);
        std::string BEC = r[BECIndex];
        std::string species = r[speciesIndex];
        Polygon P(ID, SI, BEC, species);
        polygons.push_back(P);
    }
    return(polygons);
}
