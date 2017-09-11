#include <vector>
#include "SiteIndex.h"
#include "Polygon.h"
#include "GrowthCurve.h"
#include "Logger.h"
#include "Tools.h"

void fixSiteIndexOutOfRange(Polygon &p, std::vector<GrowthCurve> &matchingCurves);
void fixMissingSiteIndex(Polygon &p, std::vector<GrowthCurve> &matchingCurves);

int fixSiteIndex(Polygon &p, const std::vector<GrowthCurve> &curves){
    std::vector<GrowthCurve> matchingCurves;
    for (auto &gc:curves){
        if (matchingSpeciesBEC(gc, p))
            matchingCurves.push_back(gc);
    }
    sortGrowthCurves(matchingCurves);
    if (matchingCurves.size() > 0){ //verify we actually found a match
        if (p.getSiteIndex() == -1) { //-1 SiteIndex means it doesn't have a site index
            fixMissingSiteIndex(p, matchingCurves);
            return(1); //return(1) means match found, polygon had a missing site index
        } else {
            fixSiteIndexOutOfRange(p, matchingCurves);
            return(2); //return(2) means match found, polygon had a site index out or range
        }
    } 
    return(0); //return(0) means no match
}

void fixSiteIndexOutOfRange(Polygon &p, std::vector<GrowthCurve> &matchingCurves){
    double siteIndex = p.getSiteIndex();

    //special case if there is only one matching curve
    if (matchingCurves.size() == 1){
        if (siteIndex < matchingCurves[0].getSI_MIN())
            p.setAU(matchingCurves[0].getAU());
    }
    int i;
    //although there are only ever 1, 2, 3, or 4 growth curves, this will work on any number.
    for (i = 0; i < matchingCurves.size() && siteIndex > matchingCurves[i].getSI_MAX(); i++);

    if (i == 0) //if i = 0 then p's site index is smaller than the smallest curve
        p.setAU(matchingCurves[i].getAU());
    else if (i == matchingCurves.size()) //if i is equal to the length of the list of curves than p's site index is greater than the greatest curve
        p.setAU(matchingCurves[i-1].getAU());
    else { //otherwise it's somewhere in between two curves
        if (siteIndex - matchingCurves[i-1].getSI_MAX() < (-1)*(siteIndex - matchingCurves[i].getSI_MIN()))
            p.setAU(matchingCurves[i-1].getAU());
        else
            p.setAU(matchingCurves[i].getAU());
    }
}


void fixMissingSiteIndex(Polygon &p, std::vector<GrowthCurve> &matchingCurves){
    //because of 0 indexing, integer division and the vector being sorted
    //this will always pick the middlemost curve or the larger of the two middlemost curves
    p.setAU(matchingCurves[matchingCurves.size()/2].getAU());
}


