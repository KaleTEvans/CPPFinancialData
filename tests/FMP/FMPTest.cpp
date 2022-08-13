#include "FinancialData/NewsData.h"
#include "FinancialData/FundamentalData.h"
#include "FinancialData/TimeConversions.h"
#include "FinancialData/Logger.h"

int main(int argc, char** argv) {
    managers::LogManager log;
    log.Initialize();

    string testIn;
    for (int i=0; i < 4; i++) testIn.push_back(argv[1][i]);
    try {
        cout << "----------" << endl;
        vector<News::NewsArticle> press = News::getPressReleases(testIn, "1");
        for (auto i : press) {
            cout << i.date << endl;
            cout << i.symbol << endl;
            cout << i.unixDate << endl;
            cout << i.title << endl;
        }
        cout << "----------" << endl;

        vector<News::NewsArticle> genNews = News::getSingleStockNews(testIn, "1");
        for (auto i : genNews) {
            cout << i.date << endl;
            cout << i.symbol << endl;
            cout << i.unixDate << endl;
            cout << i.title << endl;
            cout << i.source << endl;
        }
        cout << "----------" << endl;

        vector<Fundamentals::Earnings> earn = Fundamentals::earningsHistorical(testIn, "3");
        for (auto i : earn) {
            cout << i.date << endl;
            cout << "estimate: " << i.epsEstimate << endl;
            cout << "actual: " << i.epsActual << endl;
            cout << "----------" << endl;
        }

        
    }
    catch (web::json::json_exception& e) {
        cout << e.what() << endl;
    }

    log.Shutdown();
}