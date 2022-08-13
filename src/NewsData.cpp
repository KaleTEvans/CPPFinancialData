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

    vector<NewsArticle*> getPressReleases(const string ticker, string limit) {
        string params;

        if (limit == "") {
            params = "/press-releases/" + ticker + "?";
        } else {
            params = "/press-releases/" + ticker + "?limit=" + limit + "&";
        }
        
        json::value retVal = Connect::getJson(fmpUrl, params, fmpToken);
        vector<NewsArticle*> res;

        if (retVal.is_null()) {
            CPPFINANCIALDATA_ERROR("No press release data for ticker input: {}", ticker);
        } else {
            auto jsonArr = retVal.as_array();
            for (auto it = jsonArr.begin(); it != jsonArr.end(); ++it) {
                auto data = *it;
                json::value dataObj = data;

                string date = dataObj[U("date")].as_string();
                long unixTime = TimeConversions::convertTimeToUnix(date);
                string title = dataObj[U("title")].as_string();
                string text = dataObj[U("text")].as_string();

                NewsArticle* temp = new NewsArticle(ticker, date, unixTime, title, text);

                res.push_back(temp);
            }
        }

        return res;
    }

    vector<NewsArticle*> getSingleStockNews(const string ticker, string limit) {
        string params;

        if (limit == "") {
            params = "/stock_news?tickers=" + ticker + "&";
        } else {
            params = "/stock_news?tickers=" + ticker + "&limit=" + limit + "&";
        }
        
        json::value retVal = Connect::getJson(fmpUrl, params, fmpToken);
        vector<NewsArticle*> res;

        if (retVal.is_null()) {
            CPPFINANCIALDATA_ERROR("No news data for ticker input: {}", ticker);
        } else {
            auto jsonArr = retVal.as_array();
            for (auto it = jsonArr.begin(); it != jsonArr.end(); ++it) {
                auto data = *it;
                json::value dataObj = data;

                string date = dataObj[U("publishedDate")].as_string();
                long unixTime = TimeConversions::convertTimeToUnix(date);
                string title = dataObj[U("title")].as_string();
                string text = dataObj[U("text")].as_string();
                string source = dataObj[U("site")].as_string();

                NewsArticle* temp = new NewsArticle(ticker, date, unixTime, title, text);
                temp->source = source;

                res.push_back(temp);
            }
        }

        return res;
    }
}