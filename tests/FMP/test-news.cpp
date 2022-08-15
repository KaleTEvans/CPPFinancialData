#include "FinancialData/NewsData.h"

int main(int argc, char** argv) {
    managers::LogManager log;
    log.Initialize();

    string testIn;
    for (int i=0; i < 4; i++) testIn.push_back(argv[1][i]);

    try {
        News::NewsSentiment m3 = News::newsSentiment(testIn);
        cout << "articlesinlastweek: " << m3.articlesInLastWk << endl;
        cout << "buzz: " << m3.buzz << endl;
        cout << "weeklyaverage: " << m3.weeklyAvg << endl;
        cout << "companynewsscore: " << m3.companyNewsScore << endl;
        cout << "sectoravgbullishpct: " << m3.sectorAvgBullishPct << endl;
        cout << "sectoravgnewsscore: " << m3.sectorAvgNewsScore << endl;
        cout << "bearsentiment: " << m3.bullSentiment << endl;
        cout << "bullsentiment: " << m3.bearSentiment << endl;
        cout << "-----------" << endl;

        vector<News::NewsArticle> press = News::getPressReleases(testIn, "1");
        cout << "Press Release" << endl;
        for (auto i : press) {
            cout << i.date << endl;
            cout << i.symbol << endl;
            cout << i.unixDate << endl;
            cout << i.title << endl;
        }
        cout << "----------" << endl;

        vector<News::NewsArticle> genNews = News::getSingleStockNews(testIn, "1");
        cout << "Single Stock News" << endl;
        for (auto i : genNews) {
            cout << i.date << endl;
            cout << i.symbol << endl;
            cout << i.unixDate << endl;
            cout << i.title << endl;
            cout << i.source << endl;
        }
        cout << "----------" << endl;

        vector<News::SocialSentiment> sent = News::getSocialSentiment(testIn);
        for (int i=0; i < 3; i++) {
            cout << sent[i].date << endl;
            cout << "stocktwits posts: " << sent[i].stocktwitsPosts << endl;
            cout << "twitter posts: " << sent[i].twitterPosts << endl;
            cout << "stocktwits sent: " << sent[i].stocktwitsSentiment << endl;
            cout << "twitter sent: " << sent[i].twitterSentiment << endl;
        }
        cout << "----------" << endl;
    }
    catch (web::json::json_exception& e) {
        CPPFINANCIALDATA_ERROR(e.what());
    }

    log.Shutdown();
}