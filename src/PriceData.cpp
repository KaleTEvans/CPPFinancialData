#include "FinancialData/PriceData.h"
#include "FinancialData/TimeConversions.h"
#include "Keys.h"

namespace PriceData 
{
    FSocket::FSocket(std::vector<std::string>& tickers) {
        // Add ticker values to array
        for (auto i : tickers) ledger.push_back(i);
    }

    // When the class is deleted, close out the websocket
    FSocket::~FSocket() {
        closeSocket();
    }

    void FSocket::openSocket() {
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

    void FSocket::runSocket() {
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

    void FSocket::closeSocket() {
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
            std::cout << "ERROR: " << code << std::endl;
        }
    }

    json::value FSocket::fromString(std::string const &input) {
        json::value ret = json::value::parse(input);
        return ret;
    }

    unordered_map<string, double> getQuote(const string ticker) {
        string params = "quote?symbol=" + ticker;
        json::value retVal = Connect::getJson(finnhubUrl, params, finnhubToken);

        if (retVal.is_null() || retVal[U("c")].as_double() <= 0) CPPFINANCIALDATA_ERROR("No quote data received for ticker input: {}", ticker);

        unordered_map<string, double> res;

        // Values
        res.insert({"current", retVal[U("c")].as_double()});
        res.insert({"pctchange", retVal[U("d")].as_double()});
        res.insert({"highofday", retVal[U("h")].as_double()});
        res.insert({"lowofday", retVal[U("l")].as_double()});
        res.insert({"open", retVal[U("o")].as_double()});
        res.insert({"prevclose", retVal[U("pc")].as_double()});

        return res;
    }

    unordered_map<string, double> latestBidAsk(const string ticker) {
        string params = "stock/bidask?symbol=" + ticker;
        json::value retVal = Connect::getJson(finnhubUrl, params, finnhubToken);

        unordered_map<string, double> res;

        if (retVal.is_null() || retVal[U("a")].as_double() <= 0) CPPFINANCIALDATA_ERROR("No bid/ask data received for ticker input: {}", ticker);

        // Values
        res.insert({"ask", retVal[U("a")].as_double()});
        res.insert({"askvol", retVal[U("av")].as_double()});
        res.insert({"bid", retVal[U("b")].as_double()});
        res.insert({"bidvol", retVal[U("bv")].as_double()});
        res.insert({"time", retVal[U("t")].as_double()});

        return res;
    }
}