#include "FinancialData/FundamentalData.h"
#include "Keys.h"

namespace Fundamentals
{
    json::value getFinancialData(const string ticker, string metric) {
        // if no metric is given, provide all data points
        string params;
        if (metric != "") {
            params = "/stock/metric?symbol=" + ticker + "&metric=" + metric;
        } else {
            params = "/stock/metric?symbol=" + ticker + "&metric=all";
        }

        json::value retVal = Connect::getJson(finnhubUrl, params, privateFinnhubToken);
        if (retVal.at("metric").at("beta").is_null()) throw json::json_exception("object empty");

        return retVal;
    }

    Earnings earningsUpcoming(const string ticker) {
        time_t now = time(0);
        time_t future = now + 31536000;
        string start = TimeConversions::convertUnixToDate(now);
        string endDate = TimeConversions::convertUnixToDate(future);

        string params = "/calendar/earnings?from=" + start + "&to=" + endDate + "&symbol=" + ticker;
        json::value retVal = Connect::getJson(finnhubUrl, params, privateFinnhubToken);
        Earnings upcoming;

        if (retVal[_XPLATSTR("earningsCalendar")].as_array().size() < 1) {
            CPPFINANCIALDATA_WARN("No data received for: {}", ticker);
            throw json::json_exception("No data");
        } else {
            auto earningsArr = retVal[_XPLATSTR("earningsCalendar")].as_array();

            json::value earnings = earningsArr[earningsArr.size()-1];

            upcoming.date = earnings[_XPLATSTR("date")].as_string();
            upcoming.unixTime = TimeConversions::convertTimeToUnix(earnings[_XPLATSTR("date")].as_string());

            upcoming.epsEstimate = earnings[_XPLATSTR("epsEstimate")].as_double();
            upcoming.revenueEstimate = earnings[_XPLATSTR("revenueEstimate")].as_double();
            upcoming.quarter = earnings[_XPLATSTR("quarter")].as_double();

            // 1 : before open, 2 : after close, 3 : during 
            auto hour = earnings[_XPLATSTR("hour")].as_string();
            int timeCode;
            if (hour == "bmo") timeCode = 1;
            else if (hour == "amc") timeCode = 2;
            else if (hour == "dmh") timeCode = 3;
            else timeCode = 0;

            upcoming.timeCode = timeCode;
            upcoming.year = earnings[_XPLATSTR("year")].as_double();
        }

        return upcoming;
    }

    vector<Earnings> earningsHistorical(const string ticker, string limit) {
        string params = "/historical/earning_calendar/" + ticker + "?limit=" + limit + "&";
        json::value retVal = Connect::getJson(fmpUrl, params, fmpToken);

        vector<Earnings> res;

        if (retVal.as_array().size() < 1) {
            CPPFINANCIALDATA_WARN("No data received for: {}", ticker);
        } else {
            auto earnigsArr = retVal.as_array();

            for (auto it = earnigsArr.begin(); it != earnigsArr.end(); ++it) {
                auto data = *it;
                json::value dataObj = data;
                Earnings historical;

                historical.date = data[_XPLATSTR("date")].as_string();
                historical.unixTime = TimeConversions::convertTimeToUnix(historical.date);

                time_t now = time(0);
                if (now < historical.unixTime) continue;

                historical.epsEstimate = data[_XPLATSTR("epsEstimated")].as_double();
                historical.epsActual = data[_XPLATSTR("eps")].as_double();
                historical.revenueActual = data[_XPLATSTR("revenue")].as_double();
                historical.revenueEstimate = data[_XPLATSTR("revenueEstimated")].as_double();

                // 1 : before open, 2 : after close, 3 : during 
                auto hour = data[_XPLATSTR("time")].as_string();
                int timeCode;
                if (hour == "bmo") timeCode = 1;
                else if (hour == "amc") timeCode = 2;
                else if (hour == "dmh") timeCode = 3;
                else timeCode = 0;

                historical.timeCode = timeCode;
                res.push_back(historical);
            }
        }

        return res;
    }

    vector<SupplyChainRelations> supplyChainData(const string ticker) {
        string params = "/stock/supply-chain?symbol=" + ticker;
        json::value retVal = Connect::getJson(finnhubUrl, params, finnhubToken);

        vector<SupplyChainRelations> res;

        if (retVal[_XPLATSTR("data")].as_array().size() < 1) {
            CPPFINANCIALDATA_WARN("No data received for: {}", ticker);
        } else {
            auto supplyChainArr = retVal[_XPLATSTR("data")].as_array();

            for (auto it = supplyChainArr.begin(); it != supplyChainArr.end(); ++it) {
                auto data = *it;
                json::value dataObj = data;

                // ensure there are no null values
                if (dataObj[_XPLATSTR("country")].is_null()) continue;
                if (dataObj[_XPLATSTR("twoWeekCorrelation")].is_null()) continue;
                if (dataObj[_XPLATSTR("oneMonthCorrelation")].is_null()) continue;
                if (dataObj[_XPLATSTR("oneYearCorrelation")].is_null()) continue;

                string country = dataObj[_XPLATSTR("country")].as_string();
                float twoWkCorrelation = dataObj[_XPLATSTR("twoWeekCorrelation")].as_double();
                float oneMonthCorrelation = dataObj[_XPLATSTR("oneMonthCorrelation")].as_double();

                // only look for US suppliers
                if (country != "US") continue;
                // look for a 75% correlation or above
                if (oneMonthCorrelation < 0.8 && oneMonthCorrelation > -0.8) continue;
                if (twoWkCorrelation < 0.8 && twoWkCorrelation > -0.8) continue;

                SupplyChainRelations temp;
                temp.twoWkCorrelation = twoWkCorrelation;
                temp.oneMonthCorrelation = oneMonthCorrelation;

                temp.oneYrCorrelation = dataObj[_XPLATSTR("oneYearCorrelation")].as_double();
                temp.customer = dataObj[_XPLATSTR("customer")].as_bool();
                temp.supplier = dataObj[_XPLATSTR("supplier")].as_bool();
                temp.relatedSymbol = dataObj[_XPLATSTR("symbol")].as_string();

                res.push_back(temp);
            }
        }

        return res;
    }

    FinancialScores getFinancialScores(const string ticker) {
        string params1 = "/ratios-ttm/" + ticker + "?";
        json::value retVal1 = Connect::getJson(fmpUrl, params1, fmpToken);
        string params2 = "score?symbol=" + ticker + "&";
        json::value retVal2 = Connect::getJson("https://financialmodelingprep.com/api/v4/", params2, fmpToken);

        FinancialScores res;
        auto ret1 = retVal1.as_array();
        auto ret2 = retVal2.as_array();

        if (ret1[0][_XPLATSTR("peRatioTTM")].is_null()) {
            CPPFINANCIALDATA_WARN("No data received for: {}", ticker);
            throw json::json_exception("No data");
        } else {
            res.peRatio = ret1[0][_XPLATSTR("peRatioTTM")].as_double();
            res.pegRatio = ret1[0][_XPLATSTR("pegRatioTTM")].as_double();
            res.debtRatio = ret1[0][_XPLATSTR("debtRatioTTM")].as_double();
            res.debtEquityRatio = ret1[0][_XPLATSTR("debtEquityRatioTTM")].as_double();
            res.priceFairValue = ret1[0][_XPLATSTR("priceFairValueTTM")].as_double();
            res.netProfitMargin = ret1[0][_XPLATSTR("netProfitMarginTTM")].as_double();
            res.piotroskiScore = ret2[0][_XPLATSTR("piotroskiScore")].as_double();
        }

        return res;
    }

    vector<FinancialScores> getQuarterlyFinancialScores(const string ticker, string limit) {
        string params = "/ratios/" + ticker + "?period=quarter&limit=" + limit + "&";
        json::value retVal = Connect::getJson(fmpUrl, params, fmpToken);

        vector<FinancialScores> res;
        if (retVal.as_array().size() < 1) {
            CPPFINANCIALDATA_WARN("No data received for: {}", ticker);
        } else {
            auto scoresArr = retVal.as_array();

            for (auto it = scoresArr.begin(); it != scoresArr.end(); ++it) {
                auto data = *it;
                json::value dataObj = data;

                FinancialScores temp;

                temp.peRatio = dataObj[_XPLATSTR("priceEarningsRatio")].as_double();
                temp.pegRatio = dataObj[_XPLATSTR("priceEarningsToGrowthRatio")].as_double();
                temp.debtRatio = dataObj[_XPLATSTR("debtRatio")].as_double();
                temp.debtEquityRatio = dataObj[_XPLATSTR("debtEquityRatio")].as_double();
                temp.priceFairValue = dataObj[_XPLATSTR("priceFairValue")].as_double();
                temp.netProfitMargin = dataObj[_XPLATSTR("netProfitMargin")].as_double();
                temp.date = dataObj[_XPLATSTR("date")].as_string();
                temp.unixDate = TimeConversions::convertTimeToUnix(temp.date);

                res.push_back(temp);
            }
        }

        return res;
    }

    CompanyProfile getCompanyProfile(const string ticker) {
        string params1 = "/profile/" + ticker + "?";
        string params2 = "/stock_peers?symbol=" + ticker + "&";
        json::value retVal1 = Connect::getJson(fmpUrl, params1, fmpToken);
        json::value retVal2 = Connect::getJson("https://financialmodelingprep.com/api/v4", params2, fmpToken);

        CompanyProfile res;
        res.symbol = ticker;

        if (retVal1.as_array().size() < 1 || retVal2.as_array().size() < 1) {
            CPPFINANCIALDATA_WARN("No data received for: {}", ticker);
            throw json::json_exception("No data");
        } 

        auto tempArr1 = retVal1.as_array();
        auto tempArr2 = retVal2.as_array();
        tempArr2 = tempArr2[0][_XPLATSTR("peersList")].as_array();
        vector<string> peers;

        res.beta = tempArr1[0][_XPLATSTR("beta")].as_double();
        res.volAvg = tempArr1[0][_XPLATSTR("volAvg")].as_double();
        res.marketCap = tempArr1[0][_XPLATSTR("mktCap")].as_double();
        res.cik = tempArr1[0][_XPLATSTR("cik")].as_string();
        res.isin = tempArr1[0][_XPLATSTR("isin")].as_string();
        res.exchange = tempArr1[0][_XPLATSTR("exchange")].as_string();
        res.industry = tempArr1[0][_XPLATSTR("industry")].as_string();
        res.sector = tempArr1[0][_XPLATSTR("sector")].as_string();

        for (auto i : tempArr2) {
            peers.push_back(i.as_string());
        }

        res.peers = peers;
            
        return res;
    }

    vector<InsiderTrade> getCompanyInsiderTrades(const string ticker, string limit) {
        string params = "/insider-trading?symbol=" + ticker + "&limit=" + limit + "&";
        json::value retVal = Connect::getJson("https://financialmodelingprep.com/api/v4", params, fmpToken);

        vector<InsiderTrade> res;

        if (retVal.as_array().size() < 1) {
            CPPFINANCIALDATA_WARN("No data received for: {}", ticker);
        } else {
            auto jsonArr = retVal.as_array();
            for (auto it = jsonArr.begin(); it != jsonArr.end(); ++it) {
                auto data = *it;
                json::value dataObj = data;
                InsiderTrade temp;

                temp.symbol = ticker;
                temp.filingDate = dataObj[_XPLATSTR("filingDate")].as_string();
                temp.filingDateUnix = TimeConversions::convertTimeToUnix(temp.filingDate);
                temp.transactionDate = dataObj[_XPLATSTR("transactionDate")].as_string();
                temp.txnDateUnix = TimeConversions::convertTimeToUnix(temp.transactionDate);
                temp.transactionType = dataObj[_XPLATSTR("transactionType")].as_string();
                if (temp.transactionType == "S-Sale") temp.isSale = true;
                temp.securitiesTransacted = dataObj[_XPLATSTR("securitiesTransacted")].as_double();
                temp.price = dataObj[_XPLATSTR("price")].as_double();
                temp.totalTransaction = temp.securitiesTransacted * temp.price;

                res.push_back(temp);
            }
        }

        return res;
    }
}