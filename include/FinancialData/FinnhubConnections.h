#ifndef FINNHUBCONNECTIONS_H
#define FINNHUBCONNECTIONS_H

#include "FinnhubSocket.h"
#include <unordered_map>
#include <map>

using std::string;
using std::unordered_map;
using std::vector;

namespace PriceData
{
    /******** Stock Price Info **********/
    // Returns a hash map of price data for a specific ticker
    unordered_map<string, double> getQuote(const string ticker);
    // Returns the latest bid ask data
    unordered_map<string, double> latestBidAsk(const string ticker); 
}

namespace Fundamentals 
{
    /******* Fundamental Data Types ********/
    class SupplyChainRelations {
        public:
            SupplyChainRelations(string ticker, bool customer, bool supplier, string relatedSymbol,
                float twoWkCorrelation, float oneMonthCorrelation, float oneYrCorrelation) :
                    ticker(ticker), customer(customer), supplier(supplier), relatedSymbol(relatedSymbol),
                    twoWkCorrelation(twoWkCorrelation), oneMonthCorrelation(oneMonthCorrelation),
                    oneYrCorrelation(oneYrCorrelation) {}

            string ticker;
            bool customer;
            bool supplier;
            string relatedSymbol;
            float twoWkCorrelation;
            float oneMonthCorrelation;
            float oneYrCorrelation;
    };

    /******** Stock Fundamental Info ********/
    unordered_map<string, double> newsSentiment(const string ticker);
    std::pair<long, int> twitterMentions(const string ticker);
    // Company financials, ie P/E, high, low, etc
    // NOTE: there are several data points here, can come back and retrieve more
    json::value getFinancialData(const string ticker, string metric = "");
    // Earnings data
    unordered_map<string, double> earningsUpcoming(const string ticker);
    // Get list of company's customers and suppliers
    vector<SupplyChainRelations*> supplyChainData(const string ticker);
    
}

#endif // FINNHUBCONNECTIONS_H