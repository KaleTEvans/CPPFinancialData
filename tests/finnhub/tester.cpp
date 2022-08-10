#include "FinancialData/FinnhubConnections.h"
#include "FinancialData/Logger.h"

using std::cout;
using std::endl;

int main(int argc, char** argv) {
    managers::LogManager log;
    log.Initialize();

    string testIn;
    for (int i=0; i < 4; i++) testIn.push_back(argv[1][i]);

    try {
        PriceData::getQuote(testIn);
        PriceData::latestBidAsk(testIn);
        Fundamentals::newsSentiment(testIn);
        Fundamentals::getFinancialData(testIn);
        Fundamentals::earningsUpcoming(testIn);
        // //Fundamentals::twitterMentions(testIn);
        Fundamentals::supplyChainData(testIn);

        TechnicalData::getChartPatterns(testIn, "15");
        TechnicalData::getSupportAndResistance(testIn, "15");
        TechnicalData::getAggregateIndicators(testIn, "D");
    } 
    catch (web::json::json_exception& e) {
        cout << e.what() << endl;
    }

    log.Shutdown();
    return 0;
}