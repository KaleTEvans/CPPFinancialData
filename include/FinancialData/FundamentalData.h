#ifndef FUNDAMENTALDATA_H
#define FUNDAMENTALDATA_H

#include "FormatRequest.h"

namespace Fundamentals 
{
    /******* Finnhub Functions ********/
    class SupplyChainRelations {
        public:
            SupplyChainRelations(string ticker, bool customer, bool supplier, string relatedSymbol,
                float twoWkCorrelation, float oneMonthCorrelation, float oneYrCorrelation) :
                    ticker(ticker), customer(customer), supplier(supplier), relatedSymbol(relatedSymbol),
                    twoWkCorrelation(twoWkCorrelation), oneMonthCorrelation(oneMonthCorrelation),
                    oneYrCorrelation(oneYrCorrelation) {}
            ~SupplyChainRelations() {}

            string ticker;
            bool customer;
            bool supplier;
            string relatedSymbol;
            float twoWkCorrelation;
            float oneMonthCorrelation;
            float oneYrCorrelation;
    };

    json::value getFinancialData(const string ticker, string metric = "");
    unordered_map<string, double> earningsUpcoming(const string ticker);
    vector<SupplyChainRelations*> supplyChainData(const string ticker);
}

#endif // FUNDAMENTALDATA_H