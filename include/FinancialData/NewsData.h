#ifndef NEWSDATA_H
#define NEWSDATA_H

#include "FormatRequest.h"

namespace News
{
    struct NewsSentiment {
        int articlesInLastWk;
        double buzz;
        double weeklyAvg;
        float companyNewsScore;
        float sectorAvgBullishPct;
        float sectorAvgNewsScore;
        float bearSentiment;
        float bullSentiment;
    };
    
    struct NewsArticle {
        string symbol;
        string date;
        string localDate;
        long unixDate;
        string title;
        string text;
        string source = "";
    };

    // Get news sentiment scores from finnhub
    NewsSentiment newsSentiment(const string ticker);

    // Press releases seem to only go back to around 2019
    vector<NewsArticle> getPressReleases(const string ticker, string limit = "1");
    // News is also somewhat limited, consider using AlphaVantage as a source
    vector<NewsArticle> getSingleStockNews(const string ticker, string limit="1");
}

#endif // NEWSDATA_H