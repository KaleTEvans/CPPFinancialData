#include "FinancialData/FinnhubConnections.h"
#include "Keys.h"
#include "TimeConversions.h"
#include "FinancialData/Logger.h"

const string base = "https://finnhub.io/api/v1/";
string end = "&token=" + finnHubKey;

namespace PriceData 
{
    /******** Stock Price Info **********/
    unordered_map<string, double> getQuote(const string ticker) {
        string params = "quote?symbol=" + ticker;
        json::value retVal = Connect::getJson(base, params, end);

        if (retVal.is_null() || retVal[U("c")].as_double() <= 0) CPPFINANCIALDATA_ERROR("No quote data received for ticker input: {}", ticker);

        unordered_map<string, double> res;

        // Values
        res.insert({"current", retVal[U("c")].as_double()});
        res.insert({"pctchange", retVal[U("d")].as_double()});
        res.insert({"highofday", retVal[U("h")].as_double()});
        res.insert({"lowofday", retVal[U("l")].as_double()});
        res.insert({"open", retVal[U("o")].as_double()});
        res.insert({"prevclose", retVal[U("pc")].as_double()});

        return res;
    }

    unordered_map<string, double> latestBidAsk(const string ticker) {
        string params = "stock/bidask?symbol=" + ticker;
        json::value retVal = Connect::getJson(base, params, end);

        unordered_map<string, double> res;

        if (retVal.is_null() || retVal[U("a")].as_double() <= 0) CPPFINANCIALDATA_ERROR("No bid/ask data received for ticker input: {}", ticker);

        // Values
        res.insert({"ask", retVal[U("a")].as_double()});
        res.insert({"askvol", retVal[U("av")].as_double()});
        res.insert({"bid", retVal[U("b")].as_double()});
        res.insert({"bidvol", retVal[U("bv")].as_double()});
        res.insert({"time", retVal[U("t")].as_double()});

        return res;
    }
}

namespace Fundamentals
{
    unordered_map<string, double> newsSentiment(const string ticker) {
        string params = "/news-sentiment?symbol=" + ticker;
        json::value retVal = Connect::getJson(base, params, end);

        unordered_map<string, double> res;

        json::value buzz = retVal[U("buzz")];
        json::value sentiment = retVal[U("sentiment")];

        if (buzz.is_null()) CPPFINANCIALDATA_ERROR("No news sent data received for ticker input: {}", ticker);

        res.insert({"articlesinlastweek", buzz[U("articlesInLastWeek")].as_double()});
        res.insert({"buzz", buzz[U("buzz")].as_double()});
        res.insert({"weeklyaverage", buzz[U("weeklyAverage")].as_double()});
        res.insert({"companynewsscore", retVal[U("companyNewsScore")].as_double()});
        res.insert({"sectoravgbullishpct", retVal[U("sectorAverageBullishPercent")].as_double()});
        res.insert({"sectoravgnewsscore", retVal[U("sectorAverageNewsScore")].as_double()});
        res.insert({"bearsentiment", sentiment[U("bearishPercent")].as_double()});
        res.insert({"bullsentiment", sentiment[U("bullishPercent")].as_double()});

        return res;
    }

    json::value getFinancialData(const string ticker, string metric) {
        // if no metric is given, provide all data points
        string params;
        if (metric != "") {
            params = "/stock/metric?symbol=" + ticker + "&metric=" + metric;
        } else {
            params = "/stock/metric?symbol=" + ticker + "&metric=all";
        }

        json::value retVal = Connect::getJson(base, params, end);

        if (retVal[U("metric")].is_null()) CPPFINANCIALDATA_ERROR("No financial data received for ticker input: {}", ticker);

        return retVal;
    }

    unordered_map<string, double> earningsUpcoming(const string ticker) {
        time_t now = time(0);
        time_t future = now + 31536000;
        string start = TimeConversions::convertUnixToTime(now);
        string endDate = TimeConversions::convertUnixToTime(future);

        string params = "/calendar/earnings?from=" + start + "&to=" + endDate + "&symbol=" + ticker;
        json::value retVal = Connect::getJson(base, params, end);

        if (retVal[U("earningsCalendar")].is_null()) CPPFINANCIALDATA_ERROR("No earnings data received for ticker input: {}", ticker);

        auto earningsArr = retVal[U("earningsCalendar")].as_array();

        json::value earnings = earningsArr[earningsArr.size()-1];

        unordered_map<string, double> res;
        long time = TimeConversions::convertTimeToUnix(earnings[U("date")].as_string());

        res.insert({"date", time});
        res.insert({"epsactual", earnings[U("epsEstimate")].as_double()});
        res.insert({"quarter", earnings[U("quarter")].as_double()});

        // 1 : before open, 2 : after close, 3 : during 
        auto hour = earnings[U("hour")].as_string();
        int timeCode;
        if (hour == "bmo") timeCode = 1;
        else if (hour == "amc") timeCode = 2;
        else if (hour == "dmh") timeCode = 3;
        else timeCode = 0;

        res.insert({"timecode", timeCode});
        res.insert({"year", earnings[U("year")].as_double()});

        return res;
    }

    std::pair<long, int> twitterMentions(const string ticker) {
        string params = "/stock/social-sentiment?symbol=" + ticker;
        json::value retVal = Connect::getJson(base, params, end);

        if (retVal[U("twitter")].is_null() || retVal[U("twitter")].as_array().size() < 1) 
            CPPFINANCIALDATA_ERROR("No twitter data received for ticker input: {}", ticker);

        auto twitArr = retVal[U("twitter")].as_array();

        auto current = twitArr[0].as_object();

        string dateTime = current[U("atTime")].as_string();
        long unixTime = TimeConversions::convertTimeToUnix(dateTime);
        int mentionCount = current[U("mention")].as_integer();

        return {unixTime, mentionCount};
    }

    vector<SupplyChainRelations*> supplyChainData(const string ticker) {
        string params = "/stock/supply-chain?symbol=" + ticker;
        json::value retVal = Connect::getJson(base, params, end);

        vector<SupplyChainRelations*> res;

        if (retVal[U("data")].is_null()) CPPFINANCIALDATA_ERROR("No supply chain data received for ticker input: {}", ticker);

        auto supplyChainArr = retVal[U("data")].as_array();

        for (auto it = supplyChainArr.begin(); it != supplyChainArr.end(); ++it) {
            auto data = *it;
            json::value dataObj = data;

            // ensure there are no null values
            if (dataObj[U("country")].is_null()) continue;
            if (dataObj[U("twoWeekCorrelation")].is_null()) continue;
            if (dataObj[U("oneMonthCorrelation")].is_null()) continue;
            if (dataObj[U("oneYearCorrelation")].is_null()) continue;

            string country = dataObj[U("country")].as_string();
            float twoWkCorrelation = dataObj[U("twoWeekCorrelation")].as_double();
            float oneMonthCorrelation = dataObj[U("oneMonthCorrelation")].as_double();
            float oneYrCorrelation = dataObj[U("oneYearCorrelation")].as_double();
            bool customer = dataObj[U("customer")].as_bool();
            bool supplier = dataObj[U("supplier")].as_bool();
            string relatedSymbol = dataObj[U("symbol")].as_string();

            // only look for US suppliers
            if (country != "US") continue;
            // look for a 75% correlation or above
            if (oneMonthCorrelation < 0.8 && oneMonthCorrelation > -0.8) continue;
            if (twoWkCorrelation < 0.8 && twoWkCorrelation > -0.8) continue;

            SupplyChainRelations* temp = new SupplyChainRelations(ticker, customer, supplier, 
                relatedSymbol, twoWkCorrelation, oneMonthCorrelation, oneYrCorrelation);

            res.push_back(temp);
        }

        return res;
    }
}

namespace TechnicalData
{
    vector<ChartPatternData*> getChartPatterns(const string ticker, string resolution) {
        string params = "/scan/pattern?symbol=" + ticker + "&resolution=" + resolution;
        json::value retVal = Connect::getJson(base, params, end);

        vector<ChartPatternData*> res;

        if (retVal[U("status")].is_null()) CPPFINANCIALDATA_WARN("There is no pattern data for {} at the {} resolution", ticker, resolution);

        auto chartPatternArray = retVal[U("points")].as_array();

        for (auto it = chartPatternArray.begin(); it != chartPatternArray.end(); ++it) {
            auto data = *it;
            json::value dataObj = data;

            if (data[U("patterntype")].as_string() == "unknown") continue;
            ChartPatternData* temp = new ChartPatternData(data, ticker, resolution);

            res.push_back(temp);
        }

        return res;
    }

    vector<double> getSupportAndResistance(const string ticker, string resolution) {
        string params = "/scan/support-resistance?symbol=" + ticker + "&resolution=" + resolution;
        json::value retVal = Connect::getJson(base, params, end);
        
        vector<double> res;

        if (retVal[U("levels")].is_null()) CPPFINANCIALDATA_WARN("There is no support/resistance data for {}", ticker);

        auto levelData = retVal[U("levels")].as_array();

        for (auto it = levelData.begin(); it != levelData.end(); ++it) {
            auto data = *it;
            json::value dataObj = data;

            res.push_back(dataObj.as_double());
        }

        return res;
    }

    AggregateData getAggregateIndicators(const string ticker, string resoluition) {
        string params = "/scan/technical-indicator?symbol=" + ticker + "&resolution=" + resoluition;
        json::value retVal = Connect::getJson(base, params, end);

        if (retVal.at("technicalAnalysis").at("signal").is_null()) CPPFINANCIALDATA_WARN("There is no aggregate data for {}", ticker);

        string signal = retVal.at("technicalAnalysis").at("signal").as_string();
        double adx = retVal.at("trend").at("adx").as_double();
        bool trending = retVal.at("trend").at("trending").as_bool();

        AggregateData res(signal, adx, trending);
        return res;
    }
}
