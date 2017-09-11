#include <iostream>
#include <fstream>
#include <ctime>
#include "Logger.h"
//https://stackoverflow.com/questions/24413744/using-operator-to-write-to-both-a-file-and-cout

Logger& operator<<(Logger& l, std::ostream&(*f)(std::ostream&))
{
    l.output1 << f;
    l.output2 << f;
    return l;
}
