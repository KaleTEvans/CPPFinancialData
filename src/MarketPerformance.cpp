#include "FinancialData/MarketPerformance.h"
#include "Keys.h"

#include <unordered_map>

namespace MarketData
{
    vector<Sector> getSingleSectorMetrics(const string date) {
        string params = "/sector_price_earning_ratio?date=" + date + "&exchange=NYSE&";
        json::value retVal = Connect::getJson("https://financialmodelingprep.com/api/v4", params, fmpToken);

        vector<Sector> res;

        if (retVal.as_array().size() < 1) {
            CPPFINANCIALDATA_WARN("No data received for: {}", date);
        } else {
            auto arr = retVal.as_array();
            for (auto it = arr.begin(); it != arr.end(); ++it) {
                auto data = *it;
                json::value dataObj = data;
                Sector temp;

                temp.date = date;
                temp.sector = dataObj[_XPLATSTR("sector")].as_string();
                string pe = dataObj[_XPLATSTR("pe")].as_string();
                temp.peRatio = std::stod(pe);

                res.push_back(temp);
            }
        }

        return res;
    }

    vector<Sector> getHistoricalSectorPctChange(const string sector, string limit) {
        string params = "/historical-sectors-performance?limit=" + limit + "&";
        json::value retVal = Connect::getJson(fmpUrl, params, fmpToken);

        std::unordered_map<string, string> map;
        map["Utilities"] = "utilitiesChangesPercentage";
        map["Basic Materials"] = "basicMaterialsChangesPercentage";
        map["Communication Services"] = "communicationServicesChangesPercentage";
        map["Consumer Cyclical"] = "consumerCyclicalChangesPercentage";
        map["Consumer Defensive"] = "consumerDefensiveChangesPercentage";
        map["Energy"] = "energyChangesPercentage";
        map["Financial Services"] = "financialServicesChangesPercentage";
        map["Healthcare"] = "healthcareChangesPercentage";
        map["Industrials"] = "industrialsChangesPercentage";
        map["Real Estate"] = "realEstateChangesPercentage";
        map["Technology"] = "technologyChangesPercentage";

        vector<Sector> res;
        if (retVal.as_array().size() < 1) {
            CPPFINANCIALDATA_WARN("No data received for: {}", limit);
        } else {
            auto arr = retVal.as_array();
            for (auto it = arr.begin(); it != arr.end(); ++it) {
                auto data = *it;
                json::value dataObj = data;
                Sector temp;

                temp.date = dataObj[_XPLATSTR("date")].as_string();
                temp.changePct = dataObj[_XPLATSTR(map[sector])].as_double();
                temp.sector = sector;

                res.push_back(temp);
            }
        }

        return res;
    }

    vector<Sector> getCurrentSectorChange() {
        string params = "/sector-performance?";
        json::value retVal = Connect::getJson(fmpUrl, params, fmpToken);

        vector<Sector> res;

        if (retVal.as_array().size() < 1) {
            CPPFINANCIALDATA_WARN("No data received");
        } else {
            auto jsonArr = retVal.as_array();
            for (auto it = jsonArr.begin(); it != jsonArr.end(); ++it) {
                auto data = *it;
                json::value dataObj = data;
                Sector temp;

                if (dataObj[_XPLATSTR("sector")].as_string() == "Materials") temp.sector = "Basic Materials";
                else if (dataObj[_XPLATSTR("sector")].as_string() == "Financials") temp.sector = "Financial Services";
                else if (dataObj[_XPLATSTR("sector")].as_string() == "Health Care") temp.sector = "Healthcare";
                else if (dataObj[_XPLATSTR("sector")].as_string() == "Information Technology") temp.sector = "Technology";
                else temp.sector = dataObj[_XPLATSTR("sector")].as_string();

                temp.changePct = std::stod(dataObj[_XPLATSTR("changesPercentage")].as_string());

                res.push_back(temp);
            }
        }

        return res;
    }

    vector<NotableStock> getMostActiveStocks() {
        string params = "/stock_market/actives?";
        json::value retVal = Connect::getJson(fmpUrl, params, fmpToken);

        if (retVal.as_array().size() < 1) {
            CPPFINANCIALDATA_WARN("No data received");
            throw json::json_exception("No data");
        }

        vector<NotableStock> res;
        auto jsonArr = retVal.as_array();
        for (auto it = jsonArr.begin(); it != jsonArr.end(); ++it) {
            auto data = *it;
            json::value dataObj = data;
            NotableStock stock;

            stock.symbol = dataObj[_XPLATSTR("symbol")].as_string();
            stock.name = dataObj[_XPLATSTR("name")].as_string();
            stock.priceChange = dataObj[_XPLATSTR("change")].as_double();
            stock.price = dataObj[_XPLATSTR("price")].as_double();
            stock.changePct = dataObj[_XPLATSTR("changesPercentage")].as_double();

            res.push_back(stock);
        }

        return res;
    } 
}