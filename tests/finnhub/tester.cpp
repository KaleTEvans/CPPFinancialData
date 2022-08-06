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

    PriceData::getQuote(testIn);
    PriceData::latestBidAsk(testIn);
    Fundamentals::newsSentiment(testIn);
    Fundamentals::getFinancialData(testIn);
    Fundamentals::earningsUpcoming(testIn);
    Fundamentals::twitterMentions(testIn);
    Fundamentals::supplyChainData(testIn);

    log.Shutdown();
    return 0;
}