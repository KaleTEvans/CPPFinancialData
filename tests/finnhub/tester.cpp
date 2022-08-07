#include <iostream>
#include "FinancialData/FinnhubConnections.h"
#include "FinancialData/Logger.h"

using std::cout;
using std::endl;
using std::string;

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
        //Fundamentals::twitterMentions(testIn);
        Fundamentals::supplyChainData(testIn);

        std::vector<TechnicalData::ChartPatternData*> temp = TechnicalData::getChartPatterns(testIn, "15");
        for (auto i : temp) {
            cout << "name" << i->patternName << endl;
            cout << "type" << i->patternType << endl;
            cout << "status" << i->status << endl;
        }
    } 
    catch (std::runtime_error& e) {
        cout << e.what() << endl;
    }

    log.Shutdown();
    return 0;
}