#ifndef LOG_H
#define LOG_H

/*
 * Logger is a struct that combines two output streams into one to make outputting look a little cleaner.
 * Shamelessly inspired by the follow SO question:
 * https://stackoverflow.com/questions/24413744/using-operator-to-write-to-both-a-file-and-cout
 */
 
#include <iostream>
#include <fstream>

struct Logger
{
    Logger(std::ostream& out1, std::ostream& out2) : output1(out1), output2(out2) {}
    std::ostream& output1;
    std::ostream& output2;
};
//as far as I can figure out the implementation for the template function has to go in the header for reasons
template <typename T>
Logger& operator<<(Logger& l, T const& t)
{
    l.output1 << t;
    l.output2 << t;
    return l;
}

Logger& operator<<(Logger& l, std::ostream&(*f)(std::ostream&));

#endif