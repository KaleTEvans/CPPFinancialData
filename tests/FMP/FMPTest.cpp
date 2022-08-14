#define CPPFINANCIALDATA_ACTIVE_LEVEL CPPFINANCIALDATA_LEVEL_TRACE
#define CPPFINANCIALDATA_DEBUG_ON
#define CPPFINANCIALDATA_TRACE_ON

#include "FinancialData/NewsData.h"
#include "FinancialData/FundamentalData.h"
#include "FinancialData/TimeConversions.h"
#include "FinancialData/MacroData.h"
#include "FinancialData/MarketPerformance.h"
#include "FinancialData/Logger.h"

int main(int argc, char** argv) {
    managers::LogManager log;
    log.Initialize();

    string testIn;
    for (int i=0; i < 4; i++) testIn.push_back(argv[1][i]);
    try {
        cout << "----------" << endl;
        // vector<News::NewsArticle> press = News::getPressReleases(testIn, "1");
        // for (auto i : press) {
        //     cout << i.date << endl;
        //     cout << i.symbol << endl;
        //     cout << i.unixDate << endl;
        //     cout << i.title << endl;
        // }
        // cout << "----------" << endl;

        // vector<News::NewsArticle> genNews = News::getSingleStockNews(testIn, "1");
        // for (auto i : genNews) {
        //     cout << i.date << endl;
        //     cout << i.symbol << endl;
        //     cout << i.unixDate << endl;
        //     cout << i.title << endl;
        //     cout << i.source << endl;
        // }
        // cout << "----------" << endl;

        // vector<Fundamentals::Earnings> earn = Fundamentals::earningsHistorical(testIn, "2");
        // for (auto i : earn) {
        //     cout << i.date << endl;
        //     cout << "estimate: " << i.epsEstimate << endl;
        //     cout << "actual: " << i.epsActual << endl;
        //     cout << "----------" << endl;
        // }

        // Fundamentals::FinancialScores m1 = Fundamentals::getFinancialScores(testIn);
        // cout << "p/e: " << m1.peRatio << endl;
        // cout << "peg: " << m1.pegRatio << endl;
        // cout << "debt ratio: " << m1.debtRatio << endl;
        // cout << "piotroski: " << m1.piotroskiScore << endl;
        // cout << "----------" << endl;

        vector<Fundamentals::FinancialScores> m2 = Fundamentals::getQuarterlyFinancialScores(testIn, "2");
        for (auto i : m2) {
            cout << "p/e: " << i.peRatio << endl;
            cout << "peg: " << i.pegRatio << endl;
            cout << i.date << endl;
            cout << i.unixDate << endl;
            cout << "----------" << endl;
        }

        vector<MacroData::EconomicEvent> m3 = MacroData::getEconomicCalendar("2022-07-01", "2022-08-01");
        for (int i=0; i < 2; i++) {
            cout << "event: " << m3[i].event << endl;
            cout << "date: " << m3[i].date << endl;
            cout << "country: " << m3[i].country << endl;
            cout << "estimate: " << m3[i].estimate << endl;
            cout << "actual: " << m3[i].actual << endl;
            cout << "impact: " << m3[i].impact << endl;
            cout << "----------" << endl;
        } 

        Fundamentals::CompanyProfile m4 = Fundamentals::getCompanyProfile(testIn);
        cout << "beta: " << m4.beta << endl;
        cout << "volAvg: " << m4.volAvg << endl;
        cout << "industry: " << m4.industry << endl;
        cout << "sector: " << m4.sector << endl;
        for (int i=0; i < 2; i++) {
            cout << "peers: " << m4.peers[i] << endl;
        }
        cout << "----------" << endl;

        // vector<MarketData::Sector> m5 = MarketData::getSingleSectorMetrics("2022-08-12");
        // for (auto i : m5) {
        //     cout << i.sector << ": " << i.peRatio << endl;
        // }
        // cout << "----------" << endl;

        vector<MarketData::Sector> m6 = MarketData::getHistoricalSectorPctChange("Technology", "2");
        for (auto i : m6) {
            cout << i.date << ": " << i.changePct << endl;
        }
        cout << "----------" << endl;

        vector<MarketData::Sector> m7 = MarketData::getCurrentSectorChange();
        for (auto i : m7) {
            cout << i.sector << ": " << i.changePct << endl;
        }
        cout << "----------" << endl;

        // vector<MarketData::NotableStock> m8 = MarketData::getMostActiveStocks();
        // for (auto i : m8) {
        //     cout << i.symbol << ": " << i.priceChange << endl;
        // }

        vector<Fundamentals::InsiderTrade> m9 = Fundamentals::getCompanyInsiderTrades(testIn);
        for (int i=0; i < 2; i++) {
            cout << m9[i].filingDate << ": " << m9[i].filingDateUnix << endl;
            cout << m9[i].transactionDate << ": " << m9[i].txnDateUnix << endl;
            cout << m9[i].transactionType << ": " << m9[i].isSale << endl;
            cout << m9[i].totalTransaction << endl;
            cout << "----------" << endl;
        }

        // vector<MacroData::TreasuryRates> test = MacroData::getTreasuryRates("2022-08-12");
        // for (auto i : test) {
        //     cout << i.date << ": " << i.unixDate << endl;
        //     cout << i.oneMonth << endl;
        //     cout << "5 " << i.fiveYear << endl;
        //     cout << "2m" << i.twoMonth << endl;
        //     cout << "2y" << i.twoYear << endl;
        //     cout << "20y " << i.twentyYear << endl;
        // }   
    } 
    catch (web::json::json_exception& e) {
        CPPFINANCIALDATA_TRACE("Error: {}", e.what());
    }

    log.Shutdown();
}