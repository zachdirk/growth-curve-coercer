#ifndef SITEINDEX_H
#define SITEINDEX_H

/*
 * SiteIndex contains the functions meant to correct problems found in the Polygon joins that stem from the Site Index values.
 */

#include <vector>
#include "Polygon.h"
#include "GrowthCurve.h"

int fixSiteIndex(Polygon &p, const std::vector<GrowthCurve> &curves);

#endif