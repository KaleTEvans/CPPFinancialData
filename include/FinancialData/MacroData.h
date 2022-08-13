#ifndef MACRODATA_H
#define MACRODATA_H

#include "FormatRequest.h"

namespace MacroData
{
    class EconomicEvent {
        public:
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
    // No date inputs only gives future economic events
    vector<EconomicEvent> getEconomicCalendar(string from = "", string to = "");
}

#endif // MACRODATA_H