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

            string country = dataObj[_XPLATSTR("country")].as_string();
            string impact = dataObj[_XPLATSTR("impact")].as_string();
            if (country != "US" && country != "EU" || impact == "Low") continue;

            temp.country = country;
            temp.impact = impact;
            temp.event = dataObj[_XPLATSTR("event")].as_string();
            temp.date = dataObj[_XPLATSTR("date")].as_string();
            temp.unixTime = TimeConversions::convertTimeToUnix(temp.date);
            if (!dataObj[_XPLATSTR("actual")].is_null()) temp.actual = dataObj[_XPLATSTR("actual")].as_double();
            if (!dataObj[_XPLATSTR("previous")].is_null()) temp.previous = dataObj[_XPLATSTR("previous")].as_double();
            if (!dataObj[_XPLATSTR("change")].is_null()) temp.change = dataObj[_XPLATSTR("change")].as_double();
            if (!dataObj[_XPLATSTR("changePercentage")].is_null()) temp.changePct = dataObj[_XPLATSTR("changePercentage")].as_double();
            if (!dataObj[_XPLATSTR("estimate")].is_null()) temp.estimate = dataObj[_XPLATSTR("estimate")].as_double();

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

            temp.date = dataObj[_XPLATSTR("date")].as_string();
            temp.unixDate = TimeConversions::convertTimeToUnix(temp.date);
            temp.oneMonth = dataObj[_XPLATSTR("month1")].as_double();
            temp.twoMonth = dataObj[_XPLATSTR("month2")].as_double();
            temp.threeMonth = dataObj[_XPLATSTR("month3")].as_double();
            temp.oneYear = dataObj[_XPLATSTR("year1")].as_double();
            temp.twoYear = dataObj[_XPLATSTR("year2")].as_double();
            temp.fiveYear = dataObj[_XPLATSTR("year5")].as_double();
            temp.sevenYear = dataObj[_XPLATSTR("year7")].as_double();
            temp.tenYear = dataObj[_XPLATSTR("year10")].as_double();
            temp.twentyYear = dataObj[_XPLATSTR("year20")].as_double();
            temp.thirtyYear = dataObj[_XPLATSTR("year30")].as_double();

            res.push_back(temp);
        }

        return res;
    }

    vector<EconomicIndicator> getTreasuryHistory(string maturity, string interval) {
        string params = "/query?function=TREASURY_YIELD&interval=" + interval + "&maturity=" + maturity + "&";
        json::value retVal = Connect::getJson(alphaUrl, params, alphaKey);

        if (retVal[_XPLATSTR("data")].as_array().size() < 1) {
            CPPFINANCIALDATA_WARN("No data received for: ", maturity);
            throw json::json_exception("API Limit Reached");
        }
        
        vector<EconomicIndicator> res;
        auto jsonArr = retVal["data"].as_array();
        for (auto it = jsonArr.begin(); it != jsonArr.end(); ++it) {
            auto data = *it;
            json::value dataObj = data;

            EconomicIndicator temp;
            temp.attribute = maturity;
            temp.date = dataObj[_XPLATSTR("date")].as_string();
            temp.unixDate = TimeConversions::convertTimeToUnix(temp.date);
            string percent = dataObj[_XPLATSTR("value")].as_string();
            temp.value = std::stod(percent);

            res.push_back(temp);
        }

        return res;
    }

    vector<EconomicIndicator> getEconomicIndicator(string indicator, string from, string to) {
        if (to == "") to = TimeConversions::getCurrentDate();
        string params = "/economic?name=" + indicator + "&from=" + from + "&to=" + to + "&";
        cout << "https://financialmodelingprep.com/api/v4" + params + fmpToken << endl;
        json::value retVal = Connect::getJson("https://financialmodelingprep.com/api/v4", params, fmpToken);

        if (retVal.as_array().size() < 1) {
            CPPFINANCIALDATA_WARN("No data for: ", indicator);
            throw json::json_exception("Incorrect indicator");
        }

        vector<EconomicIndicator> res;
        auto jsonArr = retVal.as_array();
        for (auto it = jsonArr.begin(); it != jsonArr.end(); ++it) {
            auto data = *it;
            json::value dataObj = data;

            EconomicIndicator temp;
            temp.attribute = indicator;
            temp.date = dataObj["date"].as_string();
            temp.unixDate = TimeConversions::convertTimeToUnix(temp.date);
            temp.value = dataObj["value"].as_double();

            res.push_back(temp);
        }

        return res;
    }




    // ***** Macro Indicator Definitions
    string GrossDomesticProduct = "GDP";
    string realGDP = "realGDP";
    string realGDPperCapita = "realGDPperCapita";
    string federalFunds = "federalFunds";
    string ConsumerPriceIndex = "CPI";
    string inflationRate = "inflationRate";
    string inflation = "inflation";
    string retailSales = "retailSales";
    string consumerSentiment = "consumerSentiment";
    string durableGoods = "durableGoods";
    string unemploymentRate = "unemploymentRate";
    string totalNonfarmPayroll = "totalNonfarmPayroll";
    string initialClaims = "initialClaims";
    string industrialProductionTotalIndex = "industrialProductionTotalIndex";
    string newPrivatelyOwnedHousingUnitsStartedTotalUnits = "newPrivatelyOwnedHousingUnitsStartedTotalUnits";
    string totalVehicleSales = "totalVehicleSales";
    string retailMoneyFunds = "retailMoneyFunds";
    string smoothedUSRecessionProbabilities = "smoothedUSRecessionProbabilities";
    string threeMoRatesAndYieldsCertificatesOfDeposit = "3MonthOr90DayRatesAndYieldsCertificatesOfDeposit";
    string commercialBankCreditCardInterestRate = "commercialBankInterestRateOnCreditCardPlansAllAccounts";
    string thirtyYrFixedRateMortgageAvg = "30YearFixedRateMortgageAverage";
    string fifteenYrFixedRateMortgageAvg = "15YearFixedRateMortgageAverage";
    // *********
}