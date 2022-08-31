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
        client.connect(_XPLATSTR("wss://ws.finnhub.io?token=" + privateFinnHubKey)).then([](){
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
                auto priceArr = jsonObject.at(_XPLATSTR("data")).as_array();
                for (auto it = priceArr.begin(); it != priceArr.end(); ++it) {
                    auto& data = *it;
                    auto dataObj = data;

                    string ticker = dataObj[_XPLATSTR("s")].as_string();
                    long unixTime = dataObj[_XPLATSTR("t")].as_double();
                    double price = dataObj[_XPLATSTR("p")].as_double();
                    long vol = dataObj[_XPLATSTR("v")].as_double();

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

        if (retVal.is_null() || retVal[_XPLATSTR("c")].as_double() <= 0) {
            CPPFINANCIALDATA_WARN("No data received for: {}", ticker);
            throw json::json_exception("No data");
        }

        Quote res;

        // Values
        res.current = retVal[_XPLATSTR("c")].as_double();
        res.priceChange = retVal[_XPLATSTR("d")].as_double();
        res.pctChange = retVal[_XPLATSTR("dp")].as_double();
        res.high = retVal[_XPLATSTR("h")].as_double();
        res.low = retVal[_XPLATSTR("l")].as_double();
        res.open = retVal[_XPLATSTR("o")].as_double();
        res.prevClose = retVal[_XPLATSTR("pc")].as_double();
        res.unixTime = retVal[_XPLATSTR("t")].as_double();
        res.localDate = TimeConversions::convertUnixToDateTime(res.unixTime);

        return res;
    }

    BidAsk latestBidAsk(const string ticker) {
        string params = "stock/bidask?symbol=" + ticker;
        json::value retVal = Connect::getJson(finnhubUrl, params, finnhubToken);

        BidAsk res;

        if (retVal.is_null() || retVal[_XPLATSTR("a")].as_double() <= 0) {
            CPPFINANCIALDATA_WARN("No data received for: {}", ticker);
            throw json::json_exception("No data");
        }

        // Values
        res.ask = retVal[_XPLATSTR("a")].as_double();
        res.askVol = retVal[_XPLATSTR("av")].as_double();
        res.bid = retVal[_XPLATSTR("b")].as_double();
        res.bidVol = retVal[_XPLATSTR("bv")].as_double();
        res.unixTime = retVal[_XPLATSTR("t")].as_double() / 1000;
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

                temp.date = dataObj[_XPLATSTR("date")].as_string();
                temp.unixTime = TimeConversions::convertTimeToUnix(temp.date);
                temp.open = dataObj[_XPLATSTR("open")].as_double();
                temp.low = dataObj[_XPLATSTR("low")].as_double();
                temp.high = dataObj[_XPLATSTR("high")].as_double();
                temp.close = dataObj[_XPLATSTR("close")].as_double();
                temp.volume = dataObj[_XPLATSTR("volume")].as_double();

                res.push_back(temp);
            }
        }

        return res;
    }

    vector<HistoricalCandle> getDailyHistoricalData(const string ticker, string from, string to) {
        string params = "/historical-price-full/" + ticker + "?from=" + from + "&to=" + to + "&";
        cout << fmpUrl + params + fmpToken << endl;
        json::value ret = Connect::getJson(fmpUrl, params, fmpToken);
        json::array retVal = ret[_XPLATSTR("historical")].as_array();

        vector<HistoricalCandle> res;

        if (retVal.size() < 1) {
            CPPFINANCIALDATA_WARN("No data received for: {}", ticker);
        } else {
            auto jsonArr = retVal;
            for (auto it = jsonArr.begin(); it != jsonArr.end(); ++it) {
                auto data = *it;
                json::value dataObj = data;
                HistoricalCandle temp;

                temp.date = dataObj[_XPLATSTR("date")].as_string();
                temp.unixDate = TimeConversions::convertTimeToUnix(temp.date);
                temp.open = dataObj[_XPLATSTR("open")].as_double();
                temp.high = dataObj[_XPLATSTR("high")].as_double();
                temp.low = dataObj[_XPLATSTR("low")].as_double();
                temp.close = dataObj[_XPLATSTR("close")].as_double();
                temp.adjClose = dataObj[_XPLATSTR("adjClose")].as_double();
                temp.volume = dataObj[_XPLATSTR("volume")].as_double();
                temp.unadjustedVol = dataObj[_XPLATSTR("unadjustedVolume")].as_double();
                temp.change = dataObj[_XPLATSTR("change")].as_double();
                temp.changePct = dataObj[_XPLATSTR("changePercent")].as_double();
                temp.vwap = dataObj[_XPLATSTR("vwap")].as_double();

                res.push_back(temp);
            }
        }

        return res;
    }
}