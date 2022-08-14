#ifndef MARKETPERFORMANCE_H
#define MARKETPERFORMANCE_H

#include "FormatRequest.h"

namespace MarketData
{
    struct Sector {
        string date = "";
        string sector;
        double peRatio = 0;
        double changePct = 0;
    };

    struct NotableStock {
        string symbol;
        string name;
        float priceChange;
        float price;
        float changePct;
    };

    // This data can only be retrieved on a daily basis
    vector<Sector> getSingleSectorMetrics(const string date);

    // Data goes back to 3/22/20, use limit 1000
    vector<Sector> getHistoricalSectorPctChange(const string sector, string limit = "1");

    // Updated Hourly, gives percent change
    vector<Sector> getCurrentSectorChange();

    // Some additional filtering should be done here to get stocks of value
    vector<NotableStock> getMostActiveStocks();
}

#endif // MARKETPERFORMANCE_H