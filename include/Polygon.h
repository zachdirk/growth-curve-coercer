#ifndef POLYGON_H
#define POLYGON_H

/*
 * Polygon is an object meant for holding all the data that a VRI Polygon would hold.
 * Also contains functions meant to create or act on Polygons.
 */

#include <string>
#include <iostream>
#include <vector>
#include "GrowthCurve.h"
#include "Tools.h"

class Polygon{
    private:
        int mID;
        int mAU;
        double mSiteIndex;
        std::string mBEC;
        std::string mSpecies;
    public:
        //Only necessary constructor (AU defaults to -1 because there is no requirement that an input polygon have an AU already)
        Polygon(int ID, double SiteIndex, std::string BEC, std::string Species, int AU = -1);
        //Getter functions
        int getID() const { return mID; }
        int getAU() const { return mAU; }
        double getSiteIndex() const { return mSiteIndex; }
        std::string getBEC() const { return mBEC; }
        std::string getSpecies() const { return mSpecies; }
        //Setter functions (no function to set ID because the ID should be unique to the polygon and unchanging)
        Polygon& setAU(int newAU) {mAU = newAU; return *this;}
        Polygon& setSiteIndex(double newSI) {mSiteIndex = newSI; return *this;}
        Polygon& setBEC(std::string newBEC) {mBEC = newBEC; return *this;}
        Polygon& setSpecies(std::string newSpecies) {mSpecies = newSpecies; return *this;}
        //overload the output operator for easier printing
        friend std::ostream& operator<< (std::ostream &out, const Polygon &p);
        //determine if this polygon fits a given growth curve
        bool isMatch(const GrowthCurve &gc);
};



bool matchingSpeciesBEC(const GrowthCurve &gc, const Polygon &p);
bool match(Polygon &p, const std::vector<GrowthCurve> &curves);
std::vector<Polygon> makePolygonsFromCSV(const Table &polygonTable);
#endif