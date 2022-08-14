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
}