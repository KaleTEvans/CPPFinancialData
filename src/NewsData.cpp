#include "FinancialData/NewsData.h"
#include "Keys.h"

namespace News
{
    NewsSentiment newsSentiment(const string ticker) {
        string params = "/news-sentiment?symbol=" + ticker;
        json::value retVal = Connect::getJson(finnhubUrl, params, finnhubToken);

        NewsSentiment sent;

        json::value buzz = retVal[U("buzz")];
        json::value sentiment = retVal[U("sentiment")];

        if (buzz.is_null()) {
            CPPFINANCIALDATA_WARN("No data received for: {}", ticker);
            throw json::json_exception("No data");
        }

        sent.symbol = ticker;
        sent.articlesInLastWk = buzz[U("articlesInLastWeek")].as_double();
        sent.buzz = buzz[U("buzz")].as_double();
        sent.weeklyAvg = buzz[U("weeklyAverage")].as_double();
        sent.companyNewsScore = retVal[U("companyNewsScore")].as_double();
        sent.sectorAvgBullishPct = retVal[U("sectorAverageBullishPercent")].as_double();
        sent.sectorAvgNewsScore = retVal[U("sectorAverageNewsScore")].as_double();
        sent.bearSentiment = sentiment[U("bearishPercent")].as_double();
        sent.bullSentiment = sentiment[U("bullishPercent")].as_double();

        return sent;
    }

    vector<NewsArticle> getPressReleases(const string ticker, string limit) {
        string params = "/press-releases/" + ticker + "?limit=" + limit + "&";
        
        json::value retVal = Connect::getJson(fmpUrl, params, fmpToken);
        vector<NewsArticle> res;

        if (retVal.is_null()) {
            CPPFINANCIALDATA_WARN("No data received for: {}", ticker);
        } else {
            auto jsonArr = retVal.as_array();
            for (auto it = jsonArr.begin(); it != jsonArr.end(); ++it) {
                auto data = *it;
                json::value dataObj = data;

                string date = dataObj[U("date")].as_string();
                NewsArticle temp;

                temp.symbol = ticker;
                temp.unixDate = TimeConversions::convertTimeToUnix(date);
                temp.date = date;
                temp.localDate = TimeConversions::convertUnixToDateTime(temp.unixDate);

                temp.title = dataObj[U("title")].as_string();
                temp.text = dataObj[U("text")].as_string();
                
                res.push_back(temp);
            }
        }

        return res;
    }

    vector<NewsArticle> getSingleStockNews(const string ticker, string limit) {
        string params = "/stock_news?tickers=" + ticker + "&limit=" + limit + "&";
        
        json::value retVal = Connect::getJson(fmpUrl, params, fmpToken);
        vector<NewsArticle> res;

        if (retVal.is_null()) {
            CPPFINANCIALDATA_WARN("No data received for: {}", ticker);
        } else {
            auto jsonArr = retVal.as_array();
            for (auto it = jsonArr.begin(); it != jsonArr.end(); ++it) {
                auto data = *it;
                json::value dataObj = data;

                string date = dataObj[U("publishedDate")].as_string();
                NewsArticle temp;

                temp.symbol = ticker;
                temp.date = date;
                temp.unixDate = TimeConversions::convertTimeToUnix(date);
                temp.localDate = TimeConversions::convertUnixToDateTime(temp.unixDate);

                temp.title = dataObj[U("title")].as_string();
                temp.text = dataObj[U("text")].as_string();
                temp.source = dataObj[U("site")].as_string();

                res.push_back(temp);
            }
        }

        return res;
    }
}