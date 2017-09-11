#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include "GrowthCurve.h"
#include "Tools.h"

GrowthCurve::GrowthCurve(int AU, int SI_MIN, int SI_MAX, std::string BEC, std::string species){
    mAU = AU;
    mSI_MIN = SI_MIN;
    mSI_MAX = SI_MAX;
    mBEC = BEC;
    mSpecies = species;
}

std::ostream& operator<< (std::ostream &out, const GrowthCurve &gc){
    out << "This GrowthCurve has:\n";
    out << "AU: " << gc.mAU << '\n';
    out << "SI_MIN: " << gc.mSI_MIN << '\n';
    out << "SI_MAX: " << gc.mSI_MAX << '\n';
    out << "BEC: " << gc.mBEC << '\n';
    out << "Species: " << gc.mSpecies << '\n';
    return out;
}

void swap(GrowthCurve &a, GrowthCurve &b){
    GrowthCurve temp = a;
    a = b;
    b = temp;
}

void sortGrowthCurves(std::vector<GrowthCurve> &curves){
    for (int i = 0; i < curves.size(); i++){
        int min = i;
        for (int j = i+1; j < curves.size(); j++){
            if (curves[j].getSI_MIN() < curves[min].getSI_MIN())
                min = j;
        }
        swap(curves[i], curves[min]);
    }
}

bool growthCurveExists(const std::vector<GrowthCurve> &curves, const std::string &BEC, const std::string &species){
    for (auto &x:curves)
        if (caseInsensitiveStringCompare(x.getBEC(), BEC) && caseInsensitiveStringCompare(x.getSpecies(), species))
            return(true);
    
    return(false);
}

std::vector<GrowthCurve> makeCurvesFromCSV(const Table &AULut){
    Row header = AULut[0];
    std::unordered_map<std::string, int> indices = parseHeader(AULut);
    int AUIndex, minIndex, maxIndex, BECIndex, speciesIndex;
    try{
        AUIndex = findMapIndex(indices, "au");
        minIndex = findMapIndex(indices, "si_min");
        maxIndex = findMapIndex(indices, "si_max");
        BECIndex = findMapIndex(indices, "bec");
        speciesIndex = findMapIndex(indices, "ldspp");
    } catch(std::string index) {
        std::stringstream err;
        err << "Error! No " << index << " in polygon input table." << std::endl 
            << "Please ensure your table has a header, and that one field is labeled \"" << index <<"\".";
        throw err.str();
    }
    std::vector<GrowthCurve> curves;
    curves.reserve(AULut.size());
    //strange indexing to avoid the header 
    for (int i = 1; i < AULut.size(); i++){
        Row r = AULut[i];
        int AU = std::stoi(r[AUIndex]);
        int min = std::stoi(r[minIndex]);
        int max = std::stoi(r[maxIndex]);
        std::string BEC = r[BECIndex];
        std::string species = r[speciesIndex];
        GrowthCurve G(AU, min, max, BEC, species);
        curves.push_back(G);
    }
    return(curves);
}