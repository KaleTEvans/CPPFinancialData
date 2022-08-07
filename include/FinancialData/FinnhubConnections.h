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
            ~SupplyChainRelations() {}

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

namespace TechnicalData 
{
    /********* Stock Technical Analysis Data Types ***********/
    class ChartPatternData {
        public:
            ChartPatternData(json::value jsonObject, string ticker, string resolution) : 
                jsonObject(jsonObject), ticker(ticker), resolution(resolution) {
                    patternName = jsonObject[U("patternname")].as_string();
                    patternType = jsonObject[U("patterntype")].as_string();
                    status = jsonObject[U("status")].as_string();
                    sortTime = jsonObject[U("sortTime")].as_double();
                    // fill in the price points
                    json::object temp = jsonObject.as_object();
                    for (auto it = temp.cbegin(); it != temp.end(); ++it) {
                        if (it->first == "aprice" && it->second.as_double() != 0) {
                            pricePoints.push_back({"a", jsonObject[U("aprice")].as_double()});
                            timePoints.push_back(jsonObject[U("atime")].as_double());
                        } else if (it->first == "bprice" && it->second != 0) {
                            pricePoints.push_back({"b", jsonObject[U("bprice")].as_double()});
                            timePoints.push_back(jsonObject[U("btime")].as_double());
                        } else if (it->first == "cprice" && it->second != 0) {
                            pricePoints.push_back({"c", jsonObject[U("cprice")].as_double()});
                            timePoints.push_back(jsonObject[U("ctime")].as_double());
                        } else if (it->first == "dprice" && it->second != 0) {
                            pricePoints.push_back({"d", jsonObject[U("dprice")].as_double()});
                            timePoints.push_back(jsonObject[U("dtime")].as_double());
                        }
                    }

                }
            ~ChartPatternData() {}

            json::value jsonObject;
            string ticker;
            string resolution;

            string patternName;
            string patternType; // bullish or bearish or unknown
            string status; // complete or incomplete
            long sortTime;

            vector<std::pair<string, float>> pricePoints; // price of each setup point forming the pattern
            vector<long> timePoints; // price of each setup

            // the rest of the data will remain in the jsonObject
            // mostly appears to be hit or miss, these scans will likely only be useful to confirm 
            // a trade or price or need to be identified by a chart
    };

    /********* Stock Technical Analysis Data ***********/
    vector<ChartPatternData*> getChartPatterns(string ticker, string resolution);
}

#endif // FINNHUBCONNECTIONS_H