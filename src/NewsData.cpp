#include "FinancialData/NewsData.h"
#include "Keys.h"

namespace News
{
    NewsSentiment newsSentiment(const string ticker) {
        string params = "/news-sentiment?symbol=" + ticker;
        json::value retVal = Connect::getJson(finnhubUrl, params, finnhubToken);

        NewsSentiment sent;

        json::value buzz = retVal[_XPLATSTR("buzz")];
        json::value sentiment = retVal[_XPLATSTR("sentiment")];

        if (buzz.is_null()) {
            CPPFINANCIALDATA_WARN("No data received for: {}", ticker);
            throw json::json_exception("No data");
        }

        sent.symbol = ticker;
        sent.articlesInLastWk = buzz[_XPLATSTR("articlesInLastWeek")].as_double();
        sent.buzz = buzz[_XPLATSTR("buzz")].as_double();
        sent.weeklyAvg = buzz[_XPLATSTR("weeklyAverage")].as_double();
        sent.companyNewsScore = retVal[_XPLATSTR("companyNewsScore")].as_double();
        sent.sectorAvgBullishPct = retVal[_XPLATSTR("sectorAverageBullishPercent")].as_double();
        sent.sectorAvgNewsScore = retVal[_XPLATSTR("sectorAverageNewsScore")].as_double();
        sent.bearSentiment = sentiment[_XPLATSTR("bearishPercent")].as_double();
        sent.bullSentiment = sentiment[_XPLATSTR("bullishPercent")].as_double();

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

                string date = dataObj[_XPLATSTR("date")].as_string();
                NewsArticle temp;

                temp.symbol = ticker;
                temp.unixDate = TimeConversions::convertTimeToUnix(date);
                temp.date = date;
                temp.localDate = TimeConversions::convertUnixToDateTime(temp.unixDate);

                temp.title = dataObj[_XPLATSTR("title")].as_string();
                temp.text = dataObj[_XPLATSTR("text")].as_string();
                
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

                string date = dataObj[_XPLATSTR("publishedDate")].as_string();
                NewsArticle temp;

                temp.symbol = ticker;
                temp.date = date;
                temp.unixDate = TimeConversions::convertTimeToUnix(date);
                temp.localDate = TimeConversions::convertUnixToDateTime(temp.unixDate);

                temp.title = dataObj[_XPLATSTR("title")].as_string();
                temp.text = dataObj[_XPLATSTR("text")].as_string();
                temp.source = dataObj[_XPLATSTR("site")].as_string();

                res.push_back(temp);
            }
        }

        return res;
    }

    vector<SocialSentiment> getSocialSentiment(const string ticker, string page) {
        string params = "/historical/social-sentiment?symbol=" + ticker + "&page=" + page + "&";
        json::value retVal = Connect::getJson("https://financialmodelingprep.com/api/v4", params, fmpToken);

        vector<SocialSentiment> res;
        if (retVal.as_array().size() < 1) {
            // Because social traffic on certain stocks can be pretty hit or miss,
            // There will likely be a lot of errors when using this function, so will just return an empty array if there is no data
            CPPFINANCIALDATA_WARN("No data available for {} at page {}", ticker, page);
        } else {
            auto jsonArr = retVal.as_array();
            for (auto it = jsonArr.begin(); it != jsonArr.end(); ++it) {
                auto data = *it;
                json::value dataObj = data;
                
                SocialSentiment temp;
                temp.symbol = ticker;
                temp.date = dataObj[_XPLATSTR("date")].as_string();
                temp.unixDate = TimeConversions::convertTimeToUnix(temp.date);
                temp.stocktwitsPosts = dataObj[_XPLATSTR("stocktwitsPosts")].as_double();
                temp.twitterPosts = dataObj[_XPLATSTR("twitterPosts")].as_double();
                temp.stocktwitsComments = dataObj[_XPLATSTR("stocktwitsComments")].as_double();
                temp.twitterComments = dataObj[_XPLATSTR("twitterComments")].as_double();
                temp.stocktwitsLikes = dataObj[_XPLATSTR("stocktwitsLikes")].as_double();
                temp.twitterLikes = dataObj[_XPLATSTR("twitterLikes")].as_double();
                temp.stocktwitsImpressions = dataObj[_XPLATSTR("stocktwitsImpressions")].as_double();
                temp.twitterImpressions = dataObj[_XPLATSTR("twitterImpressions")].as_double();
                temp.stocktwitsSentiment = dataObj[_XPLATSTR("stocktwitsSentiment")].as_double();
                temp.twitterSentiment = dataObj[_XPLATSTR("twitterSentiment")].as_double();

                res.push_back(temp);
            }
        }

        return res;
    }
}