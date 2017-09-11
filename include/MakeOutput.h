#ifndef MAKEOUTPUT_H
#define MAKEOUTPUT_H

/*
 * MakeOutput holds the functions responsible for producing the final output documents.
 */

#include <string>
#include <vector>
#include <unordered_map>
#include "Polygon.h"
void writeIDs(const std::unordered_map<std::string, std::vector<int>> &reportMap, std::string outputDir);
void writeSummary(std::unordered_map<std::string, std::vector<int>> &reportMap, std::string outputDir);
void writeOutputCSV(const std::vector<Polygon> &polygons, std::string outputDir);
#endif