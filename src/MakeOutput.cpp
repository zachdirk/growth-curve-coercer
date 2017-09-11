#include <string>
#include <vector>
#include <unordered_map>
#include <fstream>
#include "MakeOutput.h"
#include "Polygon.h"

void writeIDs(const std::unordered_map<std::string, std::vector<int>> &reportMap, std::string outputDir){
    std::ofstream ID;
    std::string IDFile = outputDir + "IDs.txt";
    ID.open(IDFile);
    ID  << "Here is a text dump of all the problem polygon IDs (if the ID is not listed here, then it matched without the need for coercion).\n"
        << "Terms to ctrl+f (might take a couple seconds if the file size is large):\n"
        << "For polygons whose bec/species combination had no possible growth curves: noGC\n"
        << "For polygons who had their species substituted for another: subSpecies\n"
        << "For polygons who found a match directly after their species was substituted for another: subMatch\n"
        << "For polygons whose site index was out of range, but then was corrected: siteIndexOutOfRangeMatch\n"
        << "For polygons whose site index did not exist, but then was corrected: missingSiteIndexMatch\n"
        << "Note: some IDs will be present in multiple lists, this is expected.\n";

    for (auto &pair:reportMap){
        std::string key = pair.first;
        if (key == "allPolygons" || key == "noCoercion")
            continue; //no reason to print EVERY poly id or even the successful poly ids
        ID << key << "\n";
        for (auto &pid:pair.second){
            ID << pid << "\n";
        }
        ID << "\n";
    }

}

void writeSummary(std::unordered_map<std::string, std::vector<int>> &reportMap, std::string outputDir){
    std::ofstream summary;
    std::string summaryFile = outputDir + "summary.txt";
    summary.open(summaryFile);
    //I use [] instead of .at() because if there were 0 polygons in that criteria we still want to report that
    summary << "Summary polygon data:\n";
    summary << "Note: some of the fields overlap. There is no guarantee the values will sum to the number of inital polygons.\n";
    summary << "Number of polygons: "                                                                             << reportMap["allPolygons"].size() << "\n";
    summary << "Number of polygons still unmatched: "                                                             << reportMap["noMatch"].size() << "\n";
    summary << "Number of polygons with no bec zone: "                                                            << reportMap["noBEC"].size() << "\n";
    summary << "Number of polygons with no leading species: "                                                     << reportMap["noSpecies"].size() << "\n";
    summary << "Number of polygons matched without needing to be coerced: "                                       << reportMap["noCoercion"].size() << "\n";
    summary << "Number of polygons whose bec/species combination had no possible growth curves: "                 << reportMap["noGC"].size() << "\n";
    summary << "Number of polygons who had their species substituted for another: "                               << reportMap["subSpecies"].size() << "\n";
    summary << "Number of polygons who found a match directly after their species was substituted for another: "  << reportMap["subMatch"].size() << "\n";
    summary << "Number of polygons whose site index was out of range, but then was corrected: "                   << reportMap["siteIndexOutOfRangeMatch"].size() << "\n";
    summary << "Number of polygons whose site index did not exist, but then was corrected: "                      << reportMap["missingSiteIndexMatch"].size() << "\n";
}

void writeOutputCSV(const std::vector<Polygon> &polygons, std::string outputDir){
    std::ofstream matchedPolygons;
    std::string outputFile = outputDir + "matchedPolygons.csv";
    matchedPolygons.open(outputFile);
    matchedPolygons << "ID,AU,LdSpp\n"; //output fields
    for (auto &p:polygons)
        matchedPolygons << p.getID() << "," << p.getAU() << "," << p.getSpecies() << "\n";
}