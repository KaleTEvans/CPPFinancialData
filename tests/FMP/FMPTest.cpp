#include "FinancialData/NewsData.h"
#include "FinancialData/FundamentalData.h"
#include "FinancialData/TimeConversions.h"
#include "FinancialData/MacroData.h"
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

        vector<Fundamentals::Earnings> earn = Fundamentals::earningsHistorical(testIn, "2");
        for (auto i : earn) {
            cout << i.date << endl;
            cout << "estimate: " << i.epsEstimate << endl;
            cout << "actual: " << i.epsActual << endl;
            cout << "----------" << endl;
        }

        Fundamentals::FinancialScores m1 = Fundamentals::getFinancialScores(testIn);
        cout << "p/e: " << m1.peRatio << endl;
        cout << "peg: " << m1.pegRatio << endl;
        cout << "debt ratio: " << m1.debtRatio << endl;
        cout << "piotroski: " << m1.piotroskiScore << endl;
        cout << "----------" << endl;

        vector<Fundamentals::FinancialScores> m2 = Fundamentals::getQuarterlyFinancialScores(testIn, "2");
        for (auto i : m2) {
            cout << "p/e: " << i.peRatio << endl;
            cout << "peg: " << i.pegRatio << endl;
            cout << i.date << endl;
            cout << i.unixDate << endl;
            cout << "----------" << endl;
        }

        vector<MacroData::EconomicEvent> m3 = MacroData::getEconomicCalendar("2022-07-01", "2022-08-01");
        cout << m3.size() << endl;
        for (int i=0; i < 2; i++) {
            cout << "event: " << m3[i].event << endl;
            cout << "date: " << m3[i].date << endl;
            cout << "country: " << m3[i].country << endl;
            cout << "estimate: " << m3[i].estimate << endl;
            cout << "actual: " << m3[i].actual << endl;
            cout << "impact: " << m3[i].impact << endl;
            cout << "----------" << endl;
        } 
    } 
    catch (web::json::json_exception& e) {
        CPPFINANCIALDATA_ERROR("Error: {}", e.what());
    }

    log.Shutdown();
}