#ifndef FUNDAMENTALDATA_H
#define FUNDAMENTALDATA_H

#include "FormatRequest.h"

namespace Fundamentals 
{
    class Earnings {
        public:
            string date;
            long unixTime;
            double epsEstimate;
            double epsActual = 0;
            int quarter = 0;
            int timeCode;
            int year = 0;
    };
    
    class SupplyChainRelations {
        public:
            string ticker;
            bool customer;
            bool supplier;
            string relatedSymbol;
            float twoWkCorrelation;
            float oneMonthCorrelation;
            float oneYrCorrelation;
    };

    json::value getFinancialData(const string ticker, string metric = "");
    Earnings earningsUpcoming(const string ticker);
    vector<Earnings> earningsHistorical(const string ticker, string limit = "1");
    vector<SupplyChainRelations> supplyChainData(const string ticker);
}

#endif // FUNDAMENTALDATA_H