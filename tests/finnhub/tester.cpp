#include "FinancialData/PriceData.h"
#include "FinancialData/FundamentalData.h"
#include "FinancialData/TechnicalData.h"
#include "FinancialData/TimeConversions.h"
#include "FinancialData/Logger.h"

using std::cout;
using std::endl;

int main(int argc, char** argv) {
    managers::LogManager log;
    log.Initialize();

    string testIn;
    for (int i=0; i < 4; i++) testIn.push_back(argv[1][i]);

    try {
        // PriceData::getQuote(testIn);
        // PriceData::latestBidAsk(testIn);
        // Fundamentals::newsSentiment(testIn);
        // Fundamentals::getFinancialData(testIn);
        // Fundamentals::earningsUpcoming(testIn);
        // Fundamentals::supplyChainData(testIn);

        // vector<TechnicalData::ChartPatternData*> tech = TechnicalData::getChartPatterns("TSLA", "30");
        // for (auto i : tech) {
        //     cout << i->patternName << endl;
        //     cout << i->patternType << endl;
        //     cout << i->status << endl;
        //     for (int j=0; j < i->pricePoints.size(); j++) {
        //         cout << i->pricePoints[j].first << ": " << i->pricePoints[j].second << " " << TimeConversions::convertUnixToTime(i->timePoints[j]) << endl;
        //     }
        // }

        vector<string> v = {"AAPL", "TSLA"};
        PriceData::FSocket* f = new PriceData::FSocket(v);
        f->openSocket();
        sleep(5);
        delete f;

        //TechnicalData::getSupportAndResistance(testIn, "15");
        //TechnicalData::getAggregateIndicators(testIn, "D");
    } 
    catch (web::json::json_exception& e) {
        cout << e.what() << endl;
    }

    log.Shutdown();
    return 0;
}