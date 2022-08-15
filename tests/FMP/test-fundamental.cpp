#include "FinancialData/NewsData.h"
#include "FinancialData/FundamentalData.h"
#include "FinancialData/TimeConversions.h"
#include "FinancialData/MacroData.h"
#include "FinancialData/MarketPerformance.h"

int main(int argc, char** argv) {
    managers::LogManager log;
    log.Initialize();

    string testIn;
    for (int i=0; i < 4; i++) testIn.push_back(argv[1][i]);
    try {
        cout << "----------" << endl;
        
        vector<Fundamentals::Earnings> earn = Fundamentals::earningsHistorical(testIn, "2");
        cout << "Historical Earnings" << endl;
        for (auto i : earn) {
            cout << i.date << endl;
            cout << "estimate: " << i.epsEstimate << endl;
            cout << "actual: " << i.epsActual << endl;
            cout << "----------" << endl;
        }

        Fundamentals::FinancialScores m1 = Fundamentals::getFinancialScores(testIn);
        cout << "Financial Scores" << endl;
        cout << "p/e: " << m1.peRatio << endl;
        cout << "peg: " << m1.pegRatio << endl;
        cout << "debt ratio: " << m1.debtRatio << endl;
        cout << "piotroski: " << m1.piotroskiScore << endl;
        cout << "----------" << endl;

        vector<Fundamentals::FinancialScores> m2 = Fundamentals::getQuarterlyFinancialScores(testIn, "2");
        cout << "Quarterly Financials" << endl;
        for (auto i : m2) {
            cout << "p/e: " << i.peRatio << endl;
            cout << "peg: " << i.pegRatio << endl;
            cout << i.date << endl;
            cout << i.unixDate << endl;
            cout << "----------" << endl;
        }

        Fundamentals::CompanyProfile m4 = Fundamentals::getCompanyProfile(testIn);
        cout << "Company Profile" << endl;
        cout << "beta: " << m4.beta << endl;
        cout << "volAvg: " << m4.volAvg << endl;
        cout << "industry: " << m4.industry << endl;
        cout << "sector: " << m4.sector << endl;
        for (int i=0; i < 2; i++) {
            cout << "peers: " << m4.peers[i] << endl;
        }
        cout << "----------" << endl;

        vector<Fundamentals::InsiderTrade> m9 = Fundamentals::getCompanyInsiderTrades(testIn, "3");
        cout << "Insider Trades" << endl;
        for (int i=0; i < 2; i++) {
            cout << m9[i].filingDate << ": " << m9[i].filingDateUnix << endl;
            cout << m9[i].transactionDate << ": " << m9[i].txnDateUnix << endl;
            cout << m9[i].transactionType << ": " << m9[i].isSale << endl;
            cout << m9[i].totalTransaction << endl;
            cout << "----------" << endl;
        }

    } 
    catch (web::json::json_exception& e) {
        CPPFINANCIALDATA_ERROR(e.what());
    }

    log.Shutdown();
}