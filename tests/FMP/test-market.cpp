#include "FinancialData/MarketPerformance.h"

int main(int argc, char** argv) {
    managers::LogManager log;
    log.Initialize();

    string testIn;
    for (int i=0; i < 4; i++) testIn.push_back(argv[1][i]);

    try {
        vector<MarketData::Sector> m5 = MarketData::getSingleSectorMetrics("2022-08-12");
        cout << "Sector PE" << endl;
        for (auto i : m5) {
            cout << i.sector << ": " << i.peRatio << endl;
        }
        cout << "----------" << endl;

        vector<MarketData::NotableStock> m8 = MarketData::getMostActiveStocks();
        cout << "Active Stocks" << endl;
        for (int i=0; i < 5; i++) {
            cout << m8[i].symbol << ": " << m8[i].priceChange << endl;
        }
        cout << "----------" << endl;

        vector<MarketData::Sector> m7 = MarketData::getCurrentSectorChange();
        cout << "Sector % Change" << endl;
        for (auto i : m7) {
            cout << i.sector << ": " << i.changePct << endl;
        }
        cout << "----------" << endl;

        vector<MarketData::Sector> m6 = MarketData::getHistoricalSectorPctChange("Technology", "2");
        cout << "Historical Sector Change" << endl;
        for (auto i : m6) {
            cout << i.date << ": " << i.changePct << endl;
        }
        cout << "----------" << endl;
    }
    catch (web::json::json_exception& e) {
        CPPFINANCIALDATA_ERROR(e.what());
    }

    log.Shutdown();
}