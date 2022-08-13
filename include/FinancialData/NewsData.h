#ifndef NEWSDATA_H
#define NEWSDATA_H

#include "FormatRequest.h"
#include "TimeConversions.h"

namespace News
{
    class NewsArticle {
        public:
            NewsArticle(string symbol, string date, long unixDate, string title, string text) : 
                symbol(symbol), date(date), unixDate(unixDate), title(title), text(text) {
                    localDate = TimeConversions::convertUnixToTime(unixDate);
                }
            ~NewsArticle() {}

            string symbol;
            string date;
            string localDate;
            long unixDate;
            string title;
            string text;
            string source = "";
    };

    unordered_map<string, double> newsSentiment(const string ticker);
    vector<NewsArticle*> getPressReleases(const string ticker, string limit = "");
    vector<NewsArticle*> getSingleStockNews(const string ticker, string limit="");
}

#endif // NEWSDATA_H