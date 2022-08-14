#include "FinancialData/FundamentalData.h"
#include "FinancialData/FormatRequest.h"
#include "FinancialData/TimeConversions.h"
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

        if (retVal[U("metric")].is_null()) CPPFINANCIALDATA_ERROR("No financial data received for ticker input: {}", ticker);

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

        if (retVal[U("earningsCalendar")].is_null()) {
            CPPFINANCIALDATA_WARN("No earnings data received for ticker input: {}", ticker);
        } else {
            auto earningsArr = retVal[U("earningsCalendar")].as_array();

            json::value earnings = earningsArr[earningsArr.size()-1];

            upcoming.date = earnings[U("date")].as_string();
            upcoming.unixTime = TimeConversions::convertTimeToUnix(earnings[U("date")].as_string());

            upcoming.epsEstimate = earnings[U("epsEstimate")].as_double();
            upcoming.quarter = earnings[U("quarter")].as_double();

            // 1 : before open, 2 : after close, 3 : during 
            auto hour = earnings[U("hour")].as_string();
            int timeCode;
            if (hour == "bmo") timeCode = 1;
            else if (hour == "amc") timeCode = 2;
            else if (hour == "dmh") timeCode = 3;
            else timeCode = 0;

            upcoming.timeCode = timeCode;
            upcoming.year = earnings[U("year")].as_double();
        }

        return upcoming;
    }

    vector<Earnings> earningsHistorical(const string ticker, string limit) {
        string params = "/historical/earning_calendar/" + ticker + "?limit=" + limit + "&";
        json::value retVal = Connect::getJson(fmpUrl, params, fmpToken);

        vector<Earnings> res;

        if (retVal.as_array().size() < 1) {
            CPPFINANCIALDATA_ERROR("No historical earnings data received for {}", ticker);
        } else {
            auto earnigsArr = retVal.as_array();

            for (auto it = earnigsArr.begin(); it != earnigsArr.end(); ++it) {
                auto data = *it;
                json::value dataObj = data;
                Earnings historical;

                historical.date = data[U("date")].as_string();
                historical.unixTime = TimeConversions::convertTimeToUnix(historical.date);
                historical.epsEstimate = data[U("epsEstimated")].as_double();
                historical.epsActual = data[U("eps")].as_double();

                // 1 : before open, 2 : after close, 3 : during 
                auto hour = data[U("time")].as_string();
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

        if (retVal[U("data")].is_null()) {
            CPPFINANCIALDATA_WARN("No supply chain data received for ticker input: {}", ticker)
        } else {
            auto supplyChainArr = retVal[U("data")].as_array();

            for (auto it = supplyChainArr.begin(); it != supplyChainArr.end(); ++it) {
                auto data = *it;
                json::value dataObj = data;

                // ensure there are no null values
                if (dataObj[U("country")].is_null()) continue;
                if (dataObj[U("twoWeekCorrelation")].is_null()) continue;
                if (dataObj[U("oneMonthCorrelation")].is_null()) continue;
                if (dataObj[U("oneYearCorrelation")].is_null()) continue;

                string country = dataObj[U("country")].as_string();
                float twoWkCorrelation = dataObj[U("twoWeekCorrelation")].as_double();
                float oneMonthCorrelation = dataObj[U("oneMonthCorrelation")].as_double();

                // only look for US suppliers
                if (country != "US") continue;
                // look for a 75% correlation or above
                if (oneMonthCorrelation < 0.8 && oneMonthCorrelation > -0.8) continue;
                if (twoWkCorrelation < 0.8 && twoWkCorrelation > -0.8) continue;

                SupplyChainRelations temp;
                temp.twoWkCorrelation = twoWkCorrelation;
                temp.oneMonthCorrelation = oneMonthCorrelation;

                temp.oneYrCorrelation = dataObj[U("oneYearCorrelation")].as_double();
                temp.customer = dataObj[U("customer")].as_bool();
                temp.supplier = dataObj[U("supplier")].as_bool();
                temp.relatedSymbol = dataObj[U("symbol")].as_string();

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

        if (ret1[0][U("peRatioTTM")].is_null()) CPPFINANCIALDATA_ERROR("No financial scores for the ticker: {}", ticker);

        res.peRatio = ret1[0][U("peRatioTTM")].as_double();
        res.pegRatio = ret1[0][U("pegRatioTTM")].as_double();
        res.debtRatio = ret1[0][U("debtRatioTTM")].as_double();
        res.debtEquityRatio = ret1[0][U("debtEquityRatioTTM")].as_double();
        res.priceFairValue = ret1[0][U("priceFairValueTTM")].as_double();
        res.netProfitMargin = ret1[0][U("netProfitMarginTTM")].as_double();
        res.piotroskiScore = ret2[0][U("piotroskiScore")].as_double();

        return res;
    }

    vector<FinancialScores> getQuarterlyFinancialScores(const string ticker, string limit) {
        string params = "/ratios/" + ticker + "?period=quarter&limit=" + limit + "&";
        json::value retVal = Connect::getJson(fmpUrl, params, fmpToken);

        vector<FinancialScores> res;
        if (retVal.as_array().size() < 1) {
            CPPFINANCIALDATA_WARN("No quarterly financial data avaliable for {}", ticker);
        } else {
            auto scoresArr = retVal.as_array();

            for (auto it = scoresArr.begin(); it != scoresArr.end(); ++it) {
                auto data = *it;
                json::value dataObj = data;

                FinancialScores temp;

                temp.peRatio = dataObj[U("priceEarningsRatio")].as_double();
                temp.pegRatio = dataObj[U("priceEarningsToGrowthRatio")].as_double();
                temp.debtRatio = dataObj[U("debtRatio")].as_double();
                temp.debtEquityRatio = dataObj[U("debtEquityRatio")].as_double();
                temp.priceFairValue = dataObj[U("priceFairValue")].as_double();
                temp.netProfitMargin = dataObj[U("netProfitMargin")].as_double();
                temp.date = dataObj[U("date")].as_string();
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

        if (retVal1.as_array().size() < 1) CPPFINANCIALDATA_ERROR("No financial data available for {}", ticker);
        if (retVal2.as_array().size() < 1) {
            CPPFINANCIALDATA_WARN("No peers data for {}", ticker);
        } else {
            auto tempArr1 = retVal1.as_array();
            auto tempArr2 = retVal2.as_array();
            tempArr2 = tempArr2[0][U("peersList")].as_array();
            vector<string> peers;

            res.beta = tempArr1[0][U("beta")].as_double();
            res.volAvg = tempArr1[0][U("volAvg")].as_double();
            res.marketCap = tempArr1[0][U("mktCap")].as_double();
            res.cik = tempArr1[0][U("cik")].as_string();
            res.isin = tempArr1[0][U("isin")].as_string();
            res.exchange = tempArr1[0][U("exchange")].as_string();
            res.industry = tempArr1[0][U("industry")].as_string();
            res.sector = tempArr1[0][U("sector")].as_string();

            for (auto i : tempArr2) {
                peers.push_back(i.as_string());
            }

            res.peers = peers;
        }

        return res;
    }
}