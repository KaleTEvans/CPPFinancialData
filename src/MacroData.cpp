#include "FinancialData/MacroData.h"
#include "FinancialData/TimeConversions.h"
#include "FinancialData/Logger.h"
#include "Keys.h"

namespace MacroData
{
    vector<EconomicEvent> getEconomicCalendar(string from, string to) {
        string params = "/economic_calendar?from=" + from + "&to=" + to + "&";
        json::value retVal = Connect::getJson(fmpUrl, params, fmpToken);

        if (retVal.is_null()) CPPFINANCIALDATA_ERROR("No economic data found");

        auto dataArr = retVal.as_array();
        vector<EconomicEvent> res;

        for (auto it = dataArr.begin(); it != dataArr.end(); ++it) {
            auto data = *it;
            json::value dataObj = data;
            EconomicEvent temp;

            string country = dataObj[U("country")].as_string();
            string impact = dataObj[U("impact")].as_string();
            if (country != "US" && country != "EU" || impact == "Low") continue;

            temp.country = country;
            temp.impact = impact;
            temp.event = dataObj[U("event")].as_string();
            temp.date = dataObj[U("date")].as_string();
            temp.unixTime = TimeConversions::convertTimeToUnix(temp.date);
            if (!dataObj[U("actual")].is_null()) temp.actual = dataObj[U("actual")].as_double();
            if (!dataObj[U("previous")].is_null()) temp.previous = dataObj[U("previous")].as_double();
            if (!dataObj[U("change")].is_null()) temp.change = dataObj[U("change")].as_double();
            if (!dataObj[U("changePercentage")].is_null()) temp.changePct = dataObj[U("changePercentage")].as_double();
            if (!dataObj[U("estimate")].is_null()) temp.estimate = dataObj[U("estimate")].as_double();

            res.push_back(temp);
        }

        return res;
    }

    vector<Sector> getSingleSectorMetrics(const string date) {
        string params = "/sector_price_earning_ratio?date=" + date + "&exchange=NYSE&";
        json::value retVal = Connect::getJson("https://financialmodelingprep.com/api/v4", params, fmpToken);

        vector<Sector> res;

        if (retVal.as_array().size() < 1) {
            CPPFINANCIALDATA_ERROR("Could not retrieve sector p/e data for {}", date);
        } else {
            auto arr = retVal.as_array();
            for (auto it = arr.begin(); it != arr.end(); ++it) {
                auto data = *it;
                json::value dataObj = data;
                Sector temp;

                temp.date = date;
                temp.sector = dataObj[U("sector")].as_string();
                string pe = dataObj[U("pe")].as_string();
                temp.peRatio = std::stod(pe);

                res.push_back(temp);
            }
        }

        return res;
    }

    vector<Sector> getHistoricalSectorPctChange(const string sector, string limit) {
        string params = "/historical-sectors-performance?limit=" + limit + "&";
        json::value retVal = Connect::getJson(fmpUrl, params, fmpToken);

        unordered_map<string, string> map;
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
            CPPFINANCIALDATA_ERROR("No historical sector data for this limit: {}", limit);
        } else {
            auto arr = retVal.as_array();
            for (auto it = arr.begin(); it != arr.end(); ++it) {
                auto data = *it;
                json::value dataObj = data;
                Sector temp;

                temp.date = dataObj[U("date")].as_string();
                temp.changePct = dataObj[U(map[sector])].as_double();
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
            CPPFINANCIALDATA_ERROR("Could not retrieve current sector data");
        } else {
            auto jsonArr = retVal.as_array();
            for (auto it = jsonArr.begin(); it != jsonArr.end(); ++it) {
                auto data = *it;
                json::value dataObj = data;
                Sector temp;

                if (dataObj[U("sector")].as_string() == "Materials") temp.sector = "Basic Materials";
                else if (dataObj[U("sector")].as_string() == "Financials") temp.sector = "Financial Services";
                else if (dataObj[U("sector")].as_string() == "Health Care") temp.sector = "Healthcare";
                else if (dataObj[U("sector")].as_string() == "Information Technology") temp.sector = "Technology";
                else temp.sector = dataObj[U("sector")].as_string();

                temp.changePct = std::stod(dataObj[U("changesPercentage")].as_string());

                res.push_back(temp);
            }
        }

        return res;
    }
}