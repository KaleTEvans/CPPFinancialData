#include "FinancialData/TechnicalData.h"
#include "Keys.h"

namespace TechnicalData
{
    vector<double> getSupportAndResistance(const string ticker, string resolution) {
        string params = "/scan/support-resistance?symbol=" + ticker + "&resolution=" + resolution;
        json::value retVal = Connect::getJson(finnhubUrl, params, finnhubToken);
        
        vector<double> res;

        if (retVal[U("levels")].as_array().size() < 1) {
            CPPFINANCIALDATA_WARN("No data received for: {}", ticker);
        } else {
            auto levelData = retVal[U("levels")].as_array();

            for (auto it = levelData.begin(); it != levelData.end(); ++it) {
                auto data = *it;
                json::value dataObj = data;

                res.push_back(dataObj.as_double());
            }
        }

        return res;
    }

    AggregateData getAggregateIndicators(const string ticker, string resoluition) {
        string params = "/scan/technical-indicator?symbol=" + ticker + "&resolution=" + resoluition;
        json::value retVal = Connect::getJson(finnhubUrl, params, finnhubToken);

        if (retVal.at("technicalAnalysis").at("signal").is_null()) {
            CPPFINANCIALDATA_WARN("No data available for: {}", ticker);
            throw json::json_exception("No data");
        }

        AggregateData res;
        res.ticker = ticker;
        res.signal = retVal.at("technicalAnalysis").at("signal").as_string();
        res.adx = retVal.at("trend").at("adx").as_double();
        res.trending = retVal.at("trend").at("trending").as_bool();

        return res;
    }
}