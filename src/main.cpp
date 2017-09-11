#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <unordered_map>
#include "SiteIndex.h"
#include "Polygon.h"
#include "GrowthCurve.h"
#include "Logger.h"
#include "Tools.h"
#include "SpeciesSubstitution.h"
#include "MakeOutput.h"

int main(int argc, char* argv[]){
    //this program requires at least two inputs (with opt codes), any less and it can't run
    if (argc < 5) {
        std::cerr   << "Incompatible number of args supplied. You must at least supply these arguments:" << std::endl 
                    << "-au <path/to/area_unit_lookup_table.csv> -p <path/to/polygons.csv>" << std::endl
                    << "See supporting documentation for full list of parameter options.";
        return(1);
    }

    //get inputs/options
    std::string AULutInput;
    std::string polyInput;
    std::string outputDir;
    std::string subInput;
    for (int i = 1; i < argc; i++){
        std::string opt = argv[i];
        std::string lOpt = stringToLowercase(opt);
        if (i + 1 != argc){
            if (lOpt == "-au") //code for au lookup table
                AULutInput = argv[++i];
            else if (lOpt == "-p") //for polygon inventory
                polyInput = argv[++i];
            else if (lOpt == "-o") //for output directory
                outputDir = argv[++i];
            else if (lOpt == "-sub") //for species substitution table
                subInput = argv[++i];
            else if (lOpt == "-q") //for quiet mode
                fclose(stdout); //close standard out so nothing gets printed to the terminal (quiet mode)
            else{
                std::cerr << opt << " is not a valid argument. Program will attempt to continue." << std::endl;
            }
        }
    }

    //verify we got the necessary inputs
    if (AULutInput.empty()){
        std::cerr   << "No argument for AU lookup table supplied. Please verify you used the code '-au' before the filepath.";
        return(1);
    } else if (polyInput.empty()){
        std::cerr   << "No argument for polygon inventory table supplied. Please verify you used the code '-p' before the filepath.";
        return(1);
    }

    //if no output directory is provided, we use the default output directory
    if (outputDir.empty())
        outputDir = "./output/"; 
    if (!(outputDir.back() == '/' || outputDir.back() == '\\')) //verify that the user given output directory ends in a slash
        outputDir = outputDir + '/';
    std::string logDir = outputDir + "MatchGrowthCurves.log";

    //initialize logging objects
    std::ofstream logFile;
    logFile.open(logDir);
    Logger stdoutLog(logFile, std::cout);
    Logger stderrLog(logFile, std::cerr);

    stdoutLog << getTime() << "Running program with following parameters: " << std::endl;
    stdoutLog << getTime() << "Polygon inventory: " << polyInput << std::endl;
    stdoutLog << getTime() << "AU lookup table: " << AULutInput << std::endl;
    if (!subInput.empty())
        stdoutLog << getTime() << "Species substitute table: " << subInput << std::endl;
    stdoutLog << getTime() << "Output directory: " << outputDir << std::endl;

    Table polygonTable;
    Table AULutTable;
    Table subTable;
    try{
        stdoutLog << getTime() << "Parsing AU lookup table." << std::endl;
        AULutTable = parseCSV(AULutInput);
        stdoutLog << getTime() << "Parsing polygon inventory." << std::endl;
        polygonTable = parseCSV(polyInput);
        if (subInput.empty()){
            stdoutLog << getTime() << "No species substitute table to parse." << std::endl;
        } else { 
            stdoutLog << getTime() << "Parsing species substitute table." << std::endl;
            subTable = parseCSV(subInput);
        }
    } catch (std::string err) {
        stderrLog << getTime() << err;
        return(1);
    } catch (...) {
        stderrLog << getTime() <<  "Unknown error trying to read input. Verify the files exist, and that the permissions on the files are valid.";
        return(1);
    }
    std::vector<GrowthCurve> curves;
    try{
        stdoutLog << getTime() << "Making curve objects out of AU lookup table information." << std::endl;
        curves =  makeCurvesFromCSV(AULutTable);
    } catch (std::string err) {
        stderrLog << getTime() << err;
        return(1);
    } catch(...){
        stderrLog << getTime() << "Unknown error trying to parse curve information from CSV." << std::endl
                         << "Please verify there are no commas within a field in the CSV.";
        return(1);
    }

    std::vector<Polygon> polygons;
    try{
        stdoutLog << getTime() << "Making polygon objects out of polygon inventory data." << std::endl;
        polygons = makePolygonsFromCSV(polygonTable);
    } catch (std::string err) {
        stderrLog << getTime() << err;
        return(1);
    } catch(...){
        stderrLog << getTime() <<  "Unknown error trying to parse polygon information from CSV." << std::endl
                         << "Please verify there are no commas within a field in the CSV.";
        return(1);
    }
    std::vector<substitutionMap> subMaps;
    if (subTable.empty()){
        stdoutLog << getTime() << "No species substitution data to make substitution map." << std::endl;
    } else {
        stdoutLog << getTime() << "Creating substitution map from species substitution data." << std::endl;
        try {
            subMaps = makeSubMapFromCSV(subTable);
        } catch (std::string err) {
            stderrLog << getTime() << err << " Program will attempt to continue anyways." << std::endl;
        } catch (...) {
            stderrLog << getTime() << "Unknown error trying to create substitution map. Program will attempt to continue anyways." << std::endl;
        }
    }
    stdoutLog << getTime() <<  "Iterating through polygon objects to try and match them to growth curves. Those that don't match will be coerced if possible." << std::endl;
    int numPolygons = polygons.size();
    bool missingSpeciesOrBec = false;
    std::unordered_map<std::string, std::vector<int>> reportMap;
    reportMap.reserve(numPolygons);
    for (int i = 0; i < numPolygons; i++){
        if ((i+1)%100000 == 0){
            stdoutLog << getTime() <<  "Finished matching " << (i+1) << " out of " << numPolygons << " polygons.\n";
        }

        Polygon &p = polygons[i];
        reportMap["allPolygons"].push_back(p.getID());
        

        if (p.getSpecies().empty()){ //if p has no species, report it and skip
            missingSpeciesOrBec = true;
            if (p.getBEC() == "") //in case it also has no bec to make sure it's reported as well
                reportMap["noBEC"].push_back(p.getID());
            reportMap["noSpecies"].push_back(p.getID());
            reportMap["noMatch"].push_back(p.getID());
            continue;
        } 
        if (p.getBEC().empty()){ //if p has no bec, report it and skip
            missingSpeciesOrBec = true;
            reportMap["noBEC"].push_back(p.getID());
            reportMap["noMatch"].push_back(p.getID());
            continue;
        }

        if (match(p, curves)){ //if you find a match, skip to the next polygon
            reportMap["noCoercion"].push_back(p.getID());
            continue;
        } 
        //the following code only runs when there is no match found
        //we check the growth curve before the site index because assigning a new curve/species may still require us to fix the site index after
        if (!growthCurveExists(curves, p.getBEC(), p.getSpecies())){ //if no growth curves exist for this species/bec, we need to find a new bec
            reportMap["noGC"].push_back(p.getID());
            //we check the submap before the graph because the submap should only be used when NO curves exist for a certain species (ie the graph will always fail)
            if (!subMaps.empty()){ //verify we have a submap
                if (subPolygon(p, subMaps)){ //this will return if p's species is in the submap
                    reportMap["subSpecies"].push_back(p.getID());
                    if (match(p, curves)){ //if the species gets substituted, maybe we can find a match now
                        reportMap["subMatch"].push_back(p.getID());
                        continue; //if you're successful, skip to the next polygon
                    }
                }
            }
        }

        //fixSiteIndex works by finding the appropriate growth curves for the species/bec and assiging the best fit directly to the polygon
        //therefore no call to match is necessary since the polygon will have its AU assigned (if it can find one)
        switch(fixSiteIndex(p, curves)){
            case 0: reportMap["noMatch"].push_back(p.getID());
                    break;
            case 1: reportMap["missingSiteIndexMatch"].push_back(p.getID());
                    break;
            case 2: reportMap["siteIndexOutOfRangeMatch"].push_back(p.getID());
                    break;
        }
    }
    //final logging processes
    stdoutLog << getTime() <<  "Finished matching polygons, " << reportMap["noMatch"].size() << " polygons had no match and were not coercible." << std::endl
              << getTime() <<  "Please view the output directory for logs and reports." << std::endl;
    if (missingSpeciesOrBec){
        stderrLog << getTime() << "WARNING! Some polygons were found to have either no BEC or no species. These problems are NOT coercable." << std:: endl
                  << getTime() << "It is encouraged that you correct your input and try again for the best possible results." << std::endl;
    }

    stdoutLog << getTime() <<  "Writing formal output to CSV." << std::endl;
    writeOutputCSV(polygons, outputDir);

    stdoutLog << getTime() << "Finished writing formal output CSV. Now writing summary report." << std::endl;
    writeSummary(reportMap, outputDir);

    stdoutLog << getTime() << "Finished writing summary report. Now listing IDs by category." << std::endl;
    writeIDs(reportMap, outputDir);

    stdoutLog << getTime() << "Finished listing IDs by category. Now exiting program." << std::endl;
    return(0);
}