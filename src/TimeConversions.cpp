#include "FinancialData/TimeConversions.h"

namespace TimeConversions 
{
    // Time Conversion Functions
    long convertTimeToUnix(string date) {
        std::tm t{};
        std::istringstream in(date);
        
        if (date.size() > 10) {
            in >> std::get_time(&t, "%Y-%m-%d %H:%M:%S");
        } else {
            in >> std::get_time(&t, "%Y-%m-%d");
        }
        if (in.fail()) throw std::runtime_error{"Failed to parse date string"};

        std::time_t time_stamp = mktime(&t);
        long res = time_stamp;
        return res;
    }

    string convertUnixToDate(time_t date) {
        char time_buf[80];
        struct tm ts;
        ts = *localtime(&date);
        strftime(time_buf, sizeof(time_buf), "%Y-%m-%d", &ts);
        string str(time_buf);
        return time_buf;
    }

    string convertUnixToDateTime(time_t date) {
        char time_buf[80];
        struct tm ts;
        ts = *localtime(&date);
        strftime(time_buf, sizeof(time_buf), "%Y-%m-%d %H:%M:%S", &ts);
        string str(time_buf);
        return time_buf;
    }

    long convertToCST(long unixTime, long seconds) {
        return unixTime - seconds;
    }

    string getDayOfWeek(string date) {
        std::stringstream s(date);
        string day;
        string time;
        s >> day >> time;

        // Get int values for date
        int year = std::stoi(day.substr(0,4));
        int month = std::stoi(day.substr(5, 2));
        int _day = std::stoi(day.substr(8, 2));

        std::vector<string> dates={"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"}; 

        static int t[] = { 0, 3, 2, 5, 0, 3,
                        5, 1, 4, 6, 2, 4 };
        year -= month < 3;
        int dow = ( year + year / 4 - year / 100 +
                year / 400 + t[month - 1] + _day) % 7;

        string result = dates[dow];
        return result;
    }

    string getCurrentDate() {
        time_t now = time(0);
        return convertUnixToDate(now);
    }
}