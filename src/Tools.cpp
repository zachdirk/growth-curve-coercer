#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <unordered_map>
#include <ctime>
#include <cstring>
#include "Tools.h"
#include "Logger.h"
#include "Polygon.h"

std::string stringToLowercase(const std::string &s){
    std::string newString = s;
    for (int i = 0; i < s.length(); i++){
        newString[i] = tolower(s[i]);
    }
    return(newString);
}

bool caseInsensitiveStringCompare(std::string a, std::string b){
    if (a.length() != b.length())
        return(false);

    for (int i = 0; i < a.length(); i++){
        if (tolower(a[i]) != tolower(b[i]))
            return(false);
    }
    return(true);
}

std::unordered_map<std::string, int> parseHeader(const Table &t){
    Row header = t[0];
    for (auto &head:header)
        head = stringToLowercase(head); //force everything to lowercase because we'll be using them as map indices
    
    std::unordered_map<std::string, int> indices;
    for (int i = 0; i < header.size(); i++){
        indices[header[i]] = i;
    }
    return(indices);
}
//this function is a lot messier than I'd like, but I never found a nice solution online
//the majority seemed to skip empty cells (especially in the last field) and couldn't escape commas
//writing my own was a good exercise but also it work exactly as I would like it to 
Table parseCSV(const std::string &fname){
    std::ifstream input(fname);
    if (!input){
        std::stringstream err;
        err << "Error opening file: \"" << fname << "\" for reading." << std::endl 
            << "Please verify the file exists, and the file path is spell correctly.";
        throw err.str();
    }
    std::string line;
    int rows = 0;
    Table t;
    while (std::getline(input, line)){ 
        char* cline = new char[line.length()+1]; //using cstring because they are guaranteed to be null terminated
        std::strcpy(cline, line.c_str());
        Row row;
        char* c = &cline[0];
        while(1){ //messy control flow but doing it this way solves the empty cell in last field problem
            std::string cell;
            if (*c == ',') {
                c = c + 1;
            } else if (*c == '"'){
                c = c + 1;
                while (*c != '"' && *c != '\0'){
                    cell.append(1, *c);
                    c = c + 1;
                }
                if (*c == '\0'){
                    std::string s("Error! Opening quote had no closing quote. Make sure quotes at the start of a cell are used exclusively for escaping commas.");
                    throw s;
                }
                c = c + 1;
                if(*c == ','){
                    c = c + 1;
                } else {
                    row.push_back(cell);
                    break;
                }
            } else if (*c == '\0'){
                row.push_back(cell);
                break;
            } else {
                while (*c != ',' && *c != '\0'){
                    cell.append(1, *c);
                    c = c + 1;
                }
                if(*c == ','){
                    c = c + 1;
                } else {
                    row.push_back(cell);
                    break;
                }
            }
            row.push_back(cell);
        }
        t.push_back(row);
    }
    return(t);
}

int findMapIndex(const std::unordered_map<std::string, int> &indices, std::string index){
    if (indices.find(index) != indices.end())
        return(indices.at(index));

    throw index;
}
std::string getTime(){
    std::time_t rawtime;
    std::time(&rawtime);
    std::tm* timeinfo = std::localtime(&rawtime);
    char buffer[40]; //strftime doesn't accept std::string type for its buffer, 40 is arbitrary but definitely sufficient.
    if (!std::strftime(buffer,40,"[%b-%d-%Y %H:%M:%S] ",timeinfo)){ //verifying that the buffer was not exceeded
        //If the buffer is somehow exceeded, print error to cerr and don't try and add the timestamp
        std::cerr << "Buffer not long enough to support this time format. Please elongate time buffer." << std::endl;
        return("");
    }
    return(buffer);
}