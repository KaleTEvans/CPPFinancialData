#include "FinancialData/PriceData.h"
#include "FinancialData/FundamentalData.h"
#include "FinancialData/TechnicalData.h"
#include "FinancialData/TimeConversions.h"
#include "FinancialData/NewsData.h"
#include "FinancialData/Logger.h"

using std::cout;
using std::endl;

int main(int argc, char** argv) {
    managers::LogManager log;
    log.Initialize();

    string testIn;
    for (int i=0; i < 4; i++) testIn.push_back(argv[1][i]);

    try {
        // cout << "-----------" << endl;
        // PriceData::Quote m1 = PriceData::getQuote(testIn);
        // cout << "Current: " << m1.current << endl;
        // cout << "% Change: " << m1.pctChange << endl;
        // cout << "Day High: " << m1.high << endl;
        // cout << m1.localDate << endl;
        // cout << "-----------" << endl;

        // PriceData::BidAsk m2 = PriceData::latestBidAsk(testIn);
        // cout << "ask: " << m2.ask<< endl;
        // cout << "askVol: " << m2.askVol << endl;
        // cout << "bidVol: " << m2.bid << endl;
        // cout << m2.localDate << endl;
        // cout << "-----------" << endl;

        // News::NewsSentiment m3 = News::newsSentiment(testIn);
        // cout << "articlesinlastweek: " << m3.articlesInLastWk << endl;
        // cout << "buzz: " << m3.buzz << endl;
        // cout << "weeklyaverage: " << m3.weeklyAvg << endl;
        // cout << "companynewsscore: " << m3.companyNewsScore << endl;
        // cout << "sectoravgbullishpct: " << m3.sectorAvgBullishPct << endl;
        // cout << "sectoravgnewsscore: " << m3.sectorAvgNewsScore << endl;
        // cout << "bearsentiment: " << m3.bullSentiment << endl;
        // cout << "bullsentiment: " << m3.bearSentiment << endl;
        // cout << "-----------" << endl;
        
        // json::value res = Fundamentals::getFinancialData(testIn);
        // json::value js = res[U("metric")];
        // cout << "10 day avg vol : " << js[U("10DayAverageTradingVolume")].as_double() << endl;
        // cout << "eps annual ttm: " << js[U("epsBasicExclExtraItemsTTM")].as_double() << endl;
        // cout << "-----------" << endl;

        // Fundamentals::Earnings upcoming = Fundamentals::earningsUpcoming(testIn);
        // cout << "date " << upcoming.date << endl;
        // cout << "epsestimate " << upcoming.epsEstimate << endl;
        // cout << "quarter " << upcoming.quarter << endl;
        // cout << "-----------" << endl;

        vector<Fundamentals::SupplyChainRelations> supply = Fundamentals::supplyChainData(testIn);
        for (auto k : supply) {
            cout << "related: " << k.relatedSymbol << endl;
            cout << "2Wk Correlation: " << k.twoWkCorrelation << endl;
            cout << "1Mo Corr: " << k.oneMonthCorrelation << endl;
            cout << "1Yr corr: " << k.oneYrCorrelation << endl; 
        }
        cout << "-----------" << endl;

        // // vector<string> v = {"AAPL", "TSLA"};
        // // PriceData::FSocket* f = new PriceData::FSocket(v);
        // // f->openSocket();
        // // sleep(5);
        // // delete f;

        // vector<double> tempV = TechnicalData::getSupportAndResistance(testIn, "60");
        // for (auto i : tempV) cout << i << endl;
        // cout << "-----------" << endl;

        // TechnicalData::AggregateData agg(TechnicalData::getAggregateIndicators(testIn, "D"));
        // cout << "signal " << agg.signal << endl;
        // cout << "adx " << agg.adx << endl;
        // cout << "Trending? " << agg.trending << endl;
    } 
    catch (web::json::json_exception& e) {
        CPPFINANCIALDATA_ERROR(e.what());
    }

    cout << "--------" << endl;

    log.Shutdown();
    return 0;
}