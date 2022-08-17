#ifndef PRICEDATA_H
#define PRICEDATA_H

#include "FormatRequest.h"

namespace PriceData
{
    // Open a continuous stream of price data for up to 10 tickers
    // **Can only connect with one socket per api key
    struct StreamQuote {
        string symbol;
        long unixTime;
        double price;
        long volume;
    };

    class FSocket {
        public:
            FSocket(std::vector<std::string>& tickers);
            ~FSocket();

            void openSocket();
            void runSocket();
            void closeSocket();

            json::value fromString(std::string const &input);

        private:
            websocket_callback_client client;
            std::string ticker;
            std::vector<std::string> ledger;
            std::unordered_map<std::string, StreamQuote> tickerPrice;
    };

    struct Quote {
        double current;
        double priceChange;
        double pctChange;
        double high;
        double low;
        double open;
        double prevClose;
        long unixTime;
        string localDate;
    };

    struct BidAsk {
        double ask;
        double askVol;
        double bid;
        double bidVol;
        long unixTime;
        string localDate;
    };

    struct Candle {
        string date;
        long unixTime;
        double open;
        double low;
        double high;
        double close;
        long volume;
    };

    Quote getQuote(const string ticker);
    BidAsk latestBidAsk(const string ticker); 
    // Will just use recent candles here, historical price and technical data are not great on FMP
    // will likely need a different source
    vector<Candle> getHistoricalPrice(const string ticker, string interval = "4hour");
}

#endif // PRICEDATA_H