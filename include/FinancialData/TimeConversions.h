#ifndef TIMECONVERSIONS_H
#define TIMECONVERSIONS_H

#include <iostream>
#include <iomanip>
#include <ctime>
#include <chrono>
#include <vector>

using std::cout;
using std::cin;
using std::endl;
using std::string;

namespace TimeConversions 
{
    // Time Conversion Functions
    long convertTimeToUnix(string date);
    string convertUnixToTime(time_t date);
    long convertToCST(long unixTime, long seconds = 18000);
    string getDayOfWeek(string date);
}

#endif // TIMECONVERSIONS_H