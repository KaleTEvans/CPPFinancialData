#include "FinancialData/FundamentalData.h"
#include "FinancialData/FormatRequest.h"
#include "FinancialData/TimeConversions.h"
#include "Keys.h"

namespace Fundamentals
{
    json::value getFinancialData(const string ticker, string metric) {
        // if no metric is given, provide all data points
        string params;
        if (metric != "") {
            params = "/stock/metric?symbol=" + ticker + "&metric=" + metric;
        } else {
            params = "/stock/metric?symbol=" + ticker + "&metric=all";
        }

        json::value retVal = Connect::getJson(finnhubUrl, params, privateFinnhubToken);

        if (retVal[U("metric")].is_null()) CPPFINANCIALDATA_ERROR("No financial data received for ticker input: {}", ticker);

        return retVal;
    }

    unordered_map<string, double> earningsUpcoming(const string ticker) {
        time_t now = time(0);
        time_t future = now + 31536000;
        string start = TimeConversions::convertUnixToDate(now);
        string endDate = TimeConversions::convertUnixToDate(future);

        string params = "/calendar/earnings?from=" + start + "&to=" + endDate + "&symbol=" + ticker;
        json::value retVal = Connect::getJson(finnhubUrl, params, privateFinnhubToken);

        if (retVal[U("earningsCalendar")].is_null()) CPPFINANCIALDATA_ERROR("No earnings data received for ticker input: {}", ticker);

        auto earningsArr = retVal[U("earningsCalendar")].as_array();

        json::value earnings = earningsArr[earningsArr.size()-1];

        unordered_map<string, double> res;
        time_t time = TimeConversions::convertTimeToUnix(earnings[U("date")].as_string());

        res.insert({"unixTime", time});
        res.insert({"epsestimate", earnings[U("epsEstimate")].as_double()});
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

    vector<SupplyChainRelations*> supplyChainData(const string ticker) {
        string params = "/stock/supply-chain?symbol=" + ticker;
        json::value retVal = Connect::getJson(finnhubUrl, params, finnhubToken);

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