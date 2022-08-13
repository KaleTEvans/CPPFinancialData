#ifndef NEWSDATA_H
#define NEWSDATA_H

#include "FormatRequest.h"
#include "TimeConversions.h"

namespace News
{
    class NewsSentiment {
        public:
            int articlesInLastWk;
            double buzz;
            double weeklyAvg;
            float companyNewsScore;
            float sectorAvgBullishPct;
            float sectorAvgNewsScore;
            float bearSentiment;
            float bullSentiment;
    };
    
    class NewsArticle {
        public:
            NewsArticle(long unixDate) : unixDate(unixDate) {
                localDate = TimeConversions::convertUnixToTime(unixDate);
            }

            string symbol;
            string date;
            string localDate;
            long unixDate;
            string title;
            string text;
            string source = "";
    };

    NewsSentiment newsSentiment(const string ticker);
    vector<NewsArticle> getPressReleases(const string ticker, string limit = "1");
    vector<NewsArticle> getSingleStockNews(const string ticker, string limit="1");
}

#endif // NEWSDATA_H