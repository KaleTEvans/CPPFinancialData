#ifndef NEWSDATA_H
#define NEWSDATA_H

#include "FormatRequest.h"

namespace News
{
    struct NewsSentiment {
        string symbol;
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

    struct SocialSentiment {
        string symbol;
        string date;
        long unixDate;
        int stocktwitsPosts;
        int twitterPosts;
        long stocktwitsComments;
        long twitterComments;
        long stocktwitsLikes;
        long twitterLikes;
        long stocktwitsImpressions;
        long twitterImpressions;
        double stocktwitsSentiment;
        double twitterSentiment;
    };

    // Get news sentiment scores from finnhub
    NewsSentiment newsSentiment(const string ticker);

    // Press releases seem to only go back to around 2019
    vector<NewsArticle> getPressReleases(const string ticker, string limit = "1");
    // News is also somewhat limited, consider using AlphaVantage as a source
    vector<NewsArticle> getSingleStockNews(const string ticker, string limit="1");

    // Limit only goes up to 100, and is per hour, using page number here as it goes back further
    // If wanting further back, loop the function on incrementing page numbers
    vector<SocialSentiment> getSocialSentiment(const string ticker, string page = "0");
}

#endif // NEWSDATA_H