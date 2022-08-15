#include "FinancialData/MacroData.h"

int main(int argc, char** argv) {
    managers::LogManager log;
    log.Initialize();

    string testIn;
    for (int i=0; i < 4; i++) testIn.push_back(argv[1][i]);

    try {
        vector<MacroData::EconomicEvent> m3 = MacroData::getEconomicCalendar("2022-08-15", "2022-09-01");
        cout << "Economic Calendar" << endl;
        for (int i=0; i < 2; i++) {
            cout << "event: " << m3[i].event << endl;
            cout << "date: " << m3[i].date << endl;
            cout << "country: " << m3[i].country << endl;
            cout << "estimate: " << m3[i].estimate << endl;
            cout << "actual: " << m3[i].actual << endl;
            cout << "impact: " << m3[i].impact << endl;
            cout << "----------" << endl;
        } 

        vector<MacroData::TreasuryRates> test = MacroData::getTreasuryRates("2022-08-12");
        cout << "Treasury Rates" << endl;
        for (auto i : test) {
            cout << i.date << ": " << i.unixDate << endl;
            cout << i.oneMonth << endl;
            cout << "5y: " << i.fiveYear << endl;
            cout << "2m: " << i.twoMonth << endl;
            cout << "2y: " << i.twoYear << endl;
            cout << "20y: " << i.twentyYear << endl;
        }   
    }
    catch (web::json::json_exception& e) {
        CPPFINANCIALDATA_ERROR(e.what());
    }

    log.Shutdown();
}