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
    // Convert YYYY-MM-DD and/or HH-MM-SS to unix
    long convertTimeToUnix(string date);
    // Cenvert unix to YYYY-MM-DD
    string convertUnixToTime(time_t date);
    // Convert to central time as default
    long convertToCST(long unixTime, long seconds = 18000);
    // Retrieve Day of Week
    string getDayOfWeek(string date);
}

#endif // TIMECONVERSIONS_H