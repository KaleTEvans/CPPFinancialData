#include "FinancialData/NewsData.h"
#include "Keys.h"

namespace News
{
    unordered_map<string, double> newsSentiment(const string ticker) {
        string params = "/news-sentiment?symbol=" + ticker;
        json::value retVal = Connect::getJson(finnhubUrl, params, finnhubToken);

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
}