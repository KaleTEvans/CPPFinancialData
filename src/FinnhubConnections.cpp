#include "FinnhubConnections.h"
#include "Keys.h"
#include "TimeConversions.h"

const string base = "https://finnhub.io/api/v1/";
string end = "&token=" + finnHubKey;

json::value getJson(string base, string link, string end) {
    json::value retVal;

    auto requestJson = http_client(U(base))
        .request(methods::GET,
            uri_builder(U(link + end)).to_string())
    // Get the response
    .then([](http_response response) {
        // Check the status code.
        if (response.status_code() != 200) {
            throw std::runtime_error("Returned " + std::to_string(response.status_code()));
        }

        // Convert the response body to JSON object.
        return response.extract_json();
    })
    // get the data field
    .then([&](json::value jsonObject) {
        retVal = jsonObject;
    });

    // Wait for the concurrent tasks to finish.
    try {
        requestJson.wait();
    } catch (const std::exception &e) {
        printf("Error exception:%s\n", e.what());
    }

    return retVal;
}

namespace PriceData 
{
    /******** Stock Price Info **********/
    unordered_map<string, double> getQuote(const string ticker) {
        string params = "quote?symbol=" + ticker;
        json::value retVal = getJson(base, params, end);

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
        json::value retVal = getJson(base, params, end);

        unordered_map<string, double> res;

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
        json::value retVal = getJson(base, params, end);

        unordered_map<string, double> res;

        json::value buzz = retVal[U("buzz")];
        json::value sentiment = retVal[U("sentiment")];

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

        json::value retVal = getJson(base, params, end);
        return retVal;
    }

    unordered_map<string, double> earningsUpcoming(const string ticker) {
        time_t now = time(0);
        time_t future = now + 31536000;
        string start = TimeConversions::convertUnixToTime(now);
        string endDate = TimeConversions::convertUnixToTime(future);

        string params = "/calendar/earnings?from=" + start + "&to=" + endDate + "&symbol=" + ticker;
        json::value retVal = getJson(base, params, end);

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
        json::value retVal = getJson(base, params, end);

        auto twitArr = retVal[U("twitter")].as_array();
        if (twitArr.size() < 1) throw std::runtime_error("Error: No data available for this ticker");

        auto current = twitArr[0].as_object();

        string dateTime = current[U("atTime")].as_string();
        long unixTime = TimeConversions::convertTimeToUnix(dateTime);
        int mentionCount = current[U("mention")].as_integer();

        return {unixTime, mentionCount};
    }

    vector<SupplyChainRelations*> supplyChainData(const string ticker) {
        string params = "/stock/supply-chain?symbol=" + ticker;
        json::value retVal = getJson(base, params, end);

        vector<SupplyChainRelations*> res;

        auto supplyChainArr = retVal[U("data")].as_array();
        if (supplyChainArr.size() < 1) throw std::runtime_error("Error: No data available for this ticker");

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
