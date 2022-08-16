#include "FinancialData/PriceData.h"

int main(int argc, char** argv) {
    managers::LogManager log;
    log.Initialize();

    string testIn;
    for (int i=0; i < 4; i++) testIn.push_back(argv[1][i]);

    vector<PriceData::Candle> test = PriceData::getHistoricalPrice("%5EVIX", "15min");
    for (int i=0; i < 10; i++) {
        cout << test[i].date << endl;
        cout << test[i].unixTime << endl;
        cout << "high: " << test[i].high << endl;
        cout << "low: " << test[i].low << endl;
        cout << "vol: " << test[i].volume << endl;
        cout << "------------" << endl;
    }
    

    log.Shutdown();
}