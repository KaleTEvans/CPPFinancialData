#include "FinancialData/MacroData.h"
#include "Keys.h"

namespace MacroData
{
    vector<EconomicEvent> getEconomicCalendar(string from, string to) {
        string params = "/economic_calendar?from=" + from + "&to=" + to + "&";
        json::value retVal = Connect::getJson(fmpUrl, params, fmpToken);

        if (retVal.as_array().size() < 1) {
            CPPFINANCIALDATA_WARN("No data received");
            throw json::json_exception("No data");
        }

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

    vector<TreasuryRates> getTreasuryRates(string from, string to) {
        if (to == "") to = TimeConversions::getCurrentDate();

        string params = "/treasury?from=" + from + "&to=" + to + "&";
        json::value retVal = Connect::getJson("https://financialmodelingprep.com/api/v4", params, fmpToken);

        if (retVal.as_array().size() < 1) {
            CPPFINANCIALDATA_WARN("No data received");
            throw json::json_exception("No data");
        }

        vector<TreasuryRates> res;
        auto jsonArr = retVal.as_array();
        for (auto it = jsonArr.begin(); it != jsonArr.end(); ++it) {
            auto data = *it;
            json::value dataObj = data;
            TreasuryRates temp;

            temp.date = dataObj[U("date")].as_string();
            temp.unixDate = TimeConversions::convertTimeToUnix(temp.date);
            temp.oneMonth = dataObj[U("month1")].as_double();
            temp.twoMonth = dataObj[U("month2")].as_double();
            temp.threeMonth = dataObj[U("month3")].as_double();
            temp.oneYear = dataObj[U("year1")].as_double();
            temp.twoYear = dataObj[U("year2")].as_double();
            temp.fiveYear = dataObj[U("year5")].as_double();
            temp.sevenYear = dataObj[U("year7")].as_double();
            temp.tenYear = dataObj[U("year10")].as_double();
            temp.twentyYear = dataObj[U("year20")].as_double();
            temp.thirtyYear = dataObj[U("year30")].as_double();

            res.push_back(temp);
        }

        return res;
    }
}