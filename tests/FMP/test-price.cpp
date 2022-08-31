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

    cout << "HISTORICAL" << endl;
    cout << "------------" << endl;

    vector<PriceData::HistoricalCandle> test2 = PriceData::getDailyHistoricalData(testIn, "2022-08-01", "2022-08-30");
    for (int i=0; i < 10; i++) {
        cout << test2[i].date << endl;
        cout << test2[i].unixDate << endl;
        cout << "open: " << test2[i].open << endl;
        cout << "close: " << test2[i].close << endl;
        cout << "high: " << test2[i].high << endl;
        cout << "Low: " << test2[i].low << endl;
        cout << "adj close: " << test2[i].adjClose << endl;
        cout << "vol: " << test2[i].volume << endl;
        cout << "unadj vol: " << test2[i].unadjustedVol << endl;
        cout << "change %: " << test2[i].changePct << endl;
        cout << "vwap: " << test2[i].vwap << endl;
        cout << "------------" << endl;
    }
    

    log.Shutdown();
}