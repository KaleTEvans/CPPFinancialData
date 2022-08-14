#ifndef MARKETPERFORMANCE_H
#define MARKETPERFORMANCE_H

#include "FormatRequest.h"

namespace MarketData
{
    class Sector {
        public:
            string date = "";
            string sector;
            double peRatio = 0;
            double changePct = 0;
    };

    // This data can only be retrieved on a daily basis
    vector<Sector> getSingleSectorMetrics(const string date);
    vector<Sector> getHistoricalSectorPctChange(const string sector, string limit = "1");
    // Updated Hourly
    vector<Sector> getCurrentSectorChange();
}

#endif // MARKETPERFORMANCE_H