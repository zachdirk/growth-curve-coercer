#ifndef GROWTHCURVE_H
#define GROWTHCURVE_H

/*
 * GrowthCurve is an object meant for holding all the data that a growth curve would hold.
 * Also contains functions meant to create growth curves or act on growth curves.
 */

#include <iostream>
#include <string>
#include <vector>
#include "Tools.h"

class GrowthCurve{
    private:
        int mAU;
        int mSI_MIN;
        int mSI_MAX;
        std::string mBEC;
        std::string mSpecies;
    public:
        //only necessary constructor
        GrowthCurve(int AU, int SI_MIN, int SI_MAX, std::string BEC, std::string species);
        //Getter functions
        int getAU() const { return mAU; }
        double getSI_MIN() const { return mSI_MIN; }
        double getSI_MAX() const { return mSI_MAX; }
        std::string getBEC() const { return mBEC; }
        std::string getSpecies() const { return mSpecies; }
        //Setter functions
        GrowthCurve& setAU(int newAU) {mAU = newAU; return *this;}
        GrowthCurve& setSI_MIN(int newSI_MIN) {mSI_MIN = newSI_MIN; return *this;}
        GrowthCurve& setSI_MAX(int newSI_MAX) {mSI_MAX = newSI_MAX; return *this;}
        GrowthCurve& setBEC(std::string newBEC) {mBEC = newBEC; return *this;}
        GrowthCurve& setSpecies(std::string newSpecies) {mSpecies = newSpecies; return *this;}
        //overload the output operator for easier printing
        friend std::ostream& operator<< (std::ostream &out, const GrowthCurve &gc);
};

void swap(GrowthCurve &a, GrowthCurve &b);
void sortGrowthCurves(std::vector<GrowthCurve> &curves);
bool growthCurveExists(const std::vector<GrowthCurve> &curves, const std::string &BEC, const std::string &species);
std::vector<GrowthCurve> makeCurvesFromCSV(const Table &AULut);
#endif