#ifndef MACRODATA_H
#define MACRODATA_H

#include "FormatRequest.h"

namespace MacroData
{
    struct EconomicEvent {
        string event;
        string date;
        string country;
        long unixTime;
        double actual = 0;
        double previous = 0;
        double change = 0;
        double changePct = 0;
        double estimate = 0;
        string impact;
};

    struct TreasuryRates {
        string date;
        string maturity = "";
        long unixDate;
        float oneMonth;
        float twoMonth;
        float threeMonth;
        float oneYear;
        float twoYear;
        float fiveYear;
        float sevenYear;
        float tenYear;
        float twentyYear;
        float thirtyYear;
    };

    // No date inputs only gives future economic events
    vector<EconomicEvent> getEconomicCalendar(string from = "", string to = "");
    // empty from will return last 30 days, data available 3 months at a time
    vector<TreasuryRates> getTreasuryRates(string from = "", string to = "");
}

#endif // MACRODATA_H