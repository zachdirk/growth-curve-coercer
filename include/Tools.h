#ifndef TOOLS_H
#define TOOLS_H

/*
 * Tools contains a bunch of useful functions that don't really fit anywhere else.
 */

#include <string>
#include <vector>
#include <unordered_map>

using Row = std::vector<std::string>;
using Table = std::vector<Row>;

std::string stringToLowercase(const std::string &s);
bool caseInsensitiveStringCompare(std::string a, std::string b);
std::unordered_map<std::string, int> parseHeader(const Table &t);
Table parseCSV(const std::string &fname);
int findMapIndex(const std::unordered_map<std::string, int> &indices, std::string index);
std::string getTime(); //returns the time (without the quotes) in "[Jan-31-1999 23:59:59] " string format
#endif