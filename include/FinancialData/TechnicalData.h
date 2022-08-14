#ifndef TECHNICALDATA_H
#define TECHNICALDATA_H

#include "FormatRequest.h"

namespace TechnicalData 
{
    struct AggregateData {
        string ticker;
        string signal;
        double adx;
        bool trending;
    };

    vector<double> getSupportAndResistance(const string ticker, string resolution = "D");
    AggregateData getAggregateIndicators(const string tickers, string resolution = "D");
}

#endif // TECHNICALDATA_H