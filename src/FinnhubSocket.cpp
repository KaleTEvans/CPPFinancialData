#include "FinancialData/FinnhubSocket.h"
#include "Keys.h"
#include "FinancialData/Logger.h"

PriceData::FSocket::FSocket(std::vector<std::string>& tickers) {
        // Add ticker values to array
        for (auto i : tickers) ledger.push_back(i);
    }

    // When the class is deleted, close out the websocket
PriceData::FSocket::~FSocket() {
    closeSocket();
}

void PriceData::FSocket::openSocket() {
    // Connect to the client and open connection
    client.connect(U("wss://ws.finnhub.io?token=" + privateFinnHubKey)).then([](){
        std::cout << "Socket Connection Established" << std::endl;
    }).wait();

    // loop over ledger to send request to client for each ticker
    for (auto symbol : ledger) {
        websocket_outgoing_message msg;
        std::string jsonMsg = "{\"type\": \"subscribe\", \"symbol\" : \"" + symbol + "\"}";
        msg.set_utf8_message(jsonMsg);
        client.send(msg).wait();
    }

    runSocket();
}

void PriceData::FSocket::runSocket() {
    // using message handler callback rather than receive in case of large amounts of data
    client.set_message_handler([&](websocket_incoming_message in_msg) {
        auto in_str = in_msg.extract_string().get();
        to_string_t(in_str);
        json::value jsonObject = fromString(in_str);

        // ensure that there is only price data coming into the stream
        if (jsonObject.has_field("data")) {
            auto priceArr = jsonObject.at(U("data")).as_array();
            for (auto it = priceArr.begin(); it != priceArr.end(); ++it) {
                auto& data = *it;
                auto dataObj = data.as_object();

                std::string name;
                double curPrice;
                
                // Retreive json values and populate to price map
                for (auto itInner = dataObj.cbegin(); itInner != dataObj.end(); ++itInner) {
                    auto& key = itInner->first;
                    if (key == "p") curPrice = itInner->second.as_double();
                    if (key == "s") {
                        name = itInner->second.as_string();
                        tickerPrice[name] = curPrice;
                        break;
                    }
                }
            }

            for (auto i : tickerPrice) {
                std::cout << i.first << ": " << i.second << std::endl;
            }
        }
    });
}

void PriceData::FSocket::closeSocket() {
    // Send message when the connection has been closed
    string_t close_reason;
    client.set_close_handler([&close_reason](websocket_close_status close_status,
        const utility::string_t& reason, const std::error_code& code) {

        std::cout << "Connection closed" << std::endl;
    });

    try {
        client.close().wait();
    }
    catch (std::error_code& code) {
        CPPFINANCIALDATA_ERROR("Issue closing FSocket");
        std::cout << "ERROR: " << code << std::endl;
    }
}

json::value PriceData::FSocket::fromString(std::string const &input) {
    json::value ret = json::value::parse(input);
    return ret;
}