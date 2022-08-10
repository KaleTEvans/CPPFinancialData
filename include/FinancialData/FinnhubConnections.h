#ifndef FINNHUBCONNECTIONS_H
#define FINNHUBCONNECTIONS_H

#include "FormatRequest.h"

namespace PriceData
{
    /******** Stock Price Info **********/
    unordered_map<string, double> getQuote(const string ticker);
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
    json::value getFinancialData(const string ticker, string metric = "");
    unordered_map<string, double> earningsUpcoming(const string ticker);
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
            string patternType; 
            string status; 
            long sortTime;

            vector<std::pair<string, float>> pricePoints; 
            vector<long> timePoints; 
    };

    class AggregateData {
        public:
            AggregateData(string signal, double adx, bool trending) :
                signal(signal), adx(adx), trending(trending) {}
            ~AggregateData() {}

            string signal;
            double adx;
            bool trending;
    };

    /********* Stock Technical Analysis Data ***********/
    vector<ChartPatternData*> getChartPatterns(const string ticker, string resolution = "D");
    vector<double> getSupportAndResistance(const string ticker, string resolution = "D");
    AggregateData getAggregateIndicators(const string tickers, string resolution = "D");
}

#endif // FINNHUBCONNECTIONS_H