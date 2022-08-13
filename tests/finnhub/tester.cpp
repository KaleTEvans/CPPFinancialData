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
    for (int i=0; i < 3; i++) testIn.push_back(argv[1][i]);

    try {
        cout << "-----------" << endl;
        unordered_map<string, double> m1 = PriceData::getQuote(testIn);
        cout << "Current: " << m1["current"] << endl;
        cout << "% Change: " << m1["pctchange"] << endl;
        cout << "Day High: " << m1["highofday"] << endl;
        cout << "-----------" << endl;

        unordered_map<string, double> m2 = PriceData::latestBidAsk(testIn);
        cout << "ask: " << m2["ask"] << endl;
        cout << "askVol: " << m2["askvol"] << endl;
        cout << "bidVol: " << m2["bidvol"] << endl;
        cout << "Unix: " << m2["time"] << endl;
        cout << "Time: " << TimeConversions::convertUnixToTime(m2["time"]) << endl;
        cout << "-----------" << endl;

        unordered_map<string, double> m3 = News::newsSentiment(testIn);
        cout << "articlesinlastweek: " << m3["articlesinlastweek"] << endl;
        cout << "buzz: " << m3["buzz"] << endl;
        cout << "weeklyaverage: " << m3["weeklyaverage"] << endl;
        cout << "companynewsscore: " << m3["companynewsscore"] << endl;
        cout << "sectoravgbullishpct: " << m3["sectoravgbullishpct"] << endl;
        cout << "sectoravgnewsscore: " << m3["sectoravgnewsscore"] << endl;
        cout << "bearsentiment: " << m3["bearsentiment"] << endl;
        cout << "bullsentiment: " << m3["bullsentiment"] << endl;
        cout << "-----------" << endl;
        
        // json::value res = Fundamentals::getFinancialData(testIn);
        // json::value js = res[U("metric")];
        // cout << "10 day avg vol : " << js[U("10DayAverageTradingVolume")].as_double() << endl;
        // cout << "eps annual ttm: " << js[U("epsBasicExclExtraItemsTTM")].as_double() << endl;
        // cout << "-----------" << endl;

        // unordered_map<string, double> map = Fundamentals::earningsUpcoming(testIn);
        // cout << "date " << TimeConversions::convertUnixToDate(map["unixTime"]) << endl;
        // cout << "epsestimate " << map["epsestimate"] << endl;
        // cout << "quarter " << map["quarter"] << endl;
        // cout << "-----------" << endl;

        // vector<Fundamentals::SupplyChainRelations*> supply = Fundamentals::supplyChainData(testIn);
        // for (auto k : supply) {
        //     cout << "related: " << k->relatedSymbol << endl;
        //     cout << "2Wk Correlation: " << k->twoWkCorrelation << endl;
        //     cout << "1Mo Corr: " << k->oneMonthCorrelation << endl;
        //     cout << "1Yr corr: " << k->oneYrCorrelation << endl; 
        // }
        // cout << "-----------" << endl;

        vector<TechnicalData::ChartPatternData*> tech = TechnicalData::getChartPatterns(testIn, "60");
        for (auto i : tech) {
            cout << i->patternName << endl;
            cout << i->patternType << endl;
            cout << i->status << endl;
            for (int j=0; j < i->pricePoints.size(); j++) {
                cout << i->pricePoints[j].first << ": " << i->pricePoints[j].second << " " << TimeConversions::convertUnixToTime(i->timePoints[j]) << endl;
            }
        }
        cout << "-----------" << endl;

        // vector<string> v = {"AAPL", "TSLA"};
        // PriceData::FSocket* f = new PriceData::FSocket(v);
        // f->openSocket();
        // sleep(5);
        // delete f;

        vector<double> tempV = TechnicalData::getSupportAndResistance(testIn, "60");
        for (auto i : tempV) cout << i << endl;
        cout << "-----------" << endl;

        TechnicalData::AggregateData agg(TechnicalData::getAggregateIndicators(testIn, "D"));
        cout << "signal " << agg.signal << endl;
        cout << "adx " << agg.adx << endl;
        cout << "Trending? " << agg.trending << endl;
    } 
    catch (web::json::json_exception& e) {
        cout << e.what() << endl;
    }

    cout << "--------" << endl;

    log.Shutdown();
    return 0;
}