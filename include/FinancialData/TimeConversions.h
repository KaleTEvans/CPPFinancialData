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

    // Converts to local date
    string convertUnixToDate(time_t date);

    // Converts to local date and time
    string convertUnixToDateTime(time_t date);

    string getDayOfWeek(string date);
    string getCurrentDate();
}

#endif // TIMECONVERSIONS_H