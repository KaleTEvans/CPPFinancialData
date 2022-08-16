#include "FinancialData/PriceData.h"
#include "Keys.h"

namespace PriceData 
{
    FSocket::FSocket(std::vector<std::string>& tickers) {
        // Add ticker values to array
        for (auto i : tickers) {
            ledger.push_back(i);
            StreamQuote q;
            q.symbol = i;
            tickerPrice[i] = q;
        }
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
                    auto dataObj = data;

                    string ticker = dataObj[U("s")].as_string();
                    long unixTime = dataObj[U("t")].as_double();
                    double price = dataObj[U("p")].as_double();
                    long vol = dataObj[U("v")].as_double();

                    auto val = tickerPrice.find(ticker);
                    if (val != tickerPrice.end()) {
                        val->second.unixTime = unixTime;
                        val->second.price = price;
                        val->second.volume = vol;
                    }
                }

                for (auto i : ledger) {
                    std::cout << i << "| time: " << tickerPrice[i].unixTime << " | price: " <<
                     tickerPrice[i].price << " | volume: " << tickerPrice[i].volume << std::endl;
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
            cout << code << endl;
        }
    }

    json::value FSocket::fromString(std::string const &input) {
        json::value ret = json::value::parse(input);
        return ret;
    }

    Quote getQuote(const string ticker) {
        string params = "quote?symbol=" + ticker;
        json::value retVal = Connect::getJson(finnhubUrl, params, privateFinnhubToken);

        if (retVal.is_null() || retVal[U("c")].as_double() <= 0) {
            CPPFINANCIALDATA_WARN("No data received for: {}", ticker);
            throw json::json_exception("No data");
        }

        Quote res;

        // Values
        res.current = retVal[U("c")].as_double();
        res.priceChange = retVal[U("d")].as_double();
        res.pctChange = retVal[U("dp")].as_double();
        res.high = retVal[U("h")].as_double();
        res.low = retVal[U("l")].as_double();
        res.open = retVal[U("o")].as_double();
        res.prevClose = retVal[U("pc")].as_double();
        res.unixTime = retVal[U("t")].as_double();
        res.localDate = TimeConversions::convertUnixToDateTime(res.unixTime);

        return res;
    }

    BidAsk latestBidAsk(const string ticker) {
        string params = "stock/bidask?symbol=" + ticker;
        json::value retVal = Connect::getJson(finnhubUrl, params, finnhubToken);

        BidAsk res;

        if (retVal.is_null() || retVal[U("a")].as_double() <= 0) {
            CPPFINANCIALDATA_WARN("No data received for: {}", ticker);
            throw json::json_exception("No data");
        }

        // Values
        res.ask = retVal[U("a")].as_double();
        res.askVol = retVal[U("av")].as_double();
        res.bid = retVal[U("b")].as_double();
        res.bidVol = retVal[U("bv")].as_double();
        res.unixTime = retVal[U("t")].as_double() / 1000;
        res.localDate = TimeConversions::convertUnixToDateTime(res.unixTime);

        return res;
    }

    vector<Candle> getHistoricalPrice(const string ticker, string interval) {
        string params = "/historical-chart/" + interval + "/" + ticker + "?";
        json::value retVal = Connect::getJson(fmpUrl, params, fmpToken);

        vector<Candle> res;

        if (retVal.as_array().size() < 1) {
            CPPFINANCIALDATA_WARN("No data received for: {}", ticker);
        } else {
            auto jsonArr = retVal.as_array();
            for (auto it = jsonArr.begin(); it != jsonArr.end(); ++it) {
                auto data = *it;
                json::value dataObj = data;
                Candle temp;

                temp.date = dataObj[U("date")].as_string();
                temp.unixTime = TimeConversions::convertTimeToUnix(temp.date);
                temp.open = dataObj[U("open")].as_double();
                temp.low = dataObj[U("low")].as_double();
                temp.high = dataObj[U("high")].as_double();
                temp.close = dataObj[U("close")].as_double();
                temp.volume = dataObj[U("volume")].as_double();

                res.push_back(temp);
            }
        }

        return res;
    }
}