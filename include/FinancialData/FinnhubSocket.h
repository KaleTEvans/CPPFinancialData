#ifndef FINNHUBSOCKET_H
#define FINNHUBSOCKET_H

#include <iostream>
#include <ctime>
#include <unordered_map>
#include <vector>
#include <cpprest/http_client.h>
#include <cpprest/filestream.h>
#include <cpprest/uri.h>
#include <cpprest/json.h>
#include <cpprest/ws_client.h>

using namespace utility;
using namespace web;
using namespace pplx;
using namespace concurrency::streams;
using namespace web::websockets::client;
using namespace utility::conversions;
using namespace web::http;
using namespace web::http::client;

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
}

#endif // FINNHUBSOCKET_H