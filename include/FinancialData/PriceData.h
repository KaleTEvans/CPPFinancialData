#ifndef PRICEDATA_H
#define PRICEDATA_H

#include "FormatRequest.h"

namespace PriceData
{
    // Open a continuous stream of price data for up to 10 tickers
    // **Can only connect with one socket per api key
    class FSocket {
        public:
            FSocket(std::vector<std::string>& tickers);
            ~FSocket();

            void openSocket();
            void runSocket();
            void closeSocket();

            json::value fromString(std::string const &input);
            void addTickerToLedger(std::string symbol);

        private:
            websocket_callback_client client;
            std::string ticker;
            std::vector<std::string> ledger;
            std::unordered_map<std::string, double> tickerPrice;
    };

    unordered_map<string, double> getQuote(const string ticker);
    unordered_map<string, double> latestBidAsk(const string ticker); 
}

#endif // PRICEDATA_H