#include "FinancialData/TechnicalData.h"
#include "FinancialData/TimeConversions.h"
#include "Keys.h"

namespace TechnicalData
{
    vector<ChartPatternData> getChartPatterns(const string ticker, string resolution) {
        string params = "/scan/pattern?symbol=" + ticker + "&resolution=" + resolution;
        json::value retVal = Connect::getJson(finnhubUrl, params, finnhubToken);

        vector<ChartPatternData> res;

        auto chartPatternArray = retVal[U("points")].as_array();

        if (chartPatternArray[0][U("status")].is_null()) {
            CPPFINANCIALDATA_WARN("There is no pattern data for {} at the {} resolution", ticker, resolution);
        } else {
            for (auto it = chartPatternArray.begin(); it != chartPatternArray.end(); ++it) {
                auto data = *it;
                json::value dataObj = data;

                if (data[U("patterntype")].as_string() == "unknown") continue;
                ChartPatternData temp(data, ticker, resolution);

                res.push_back(temp);
            }
        }

        return res;
    }

    vector<double> getSupportAndResistance(const string ticker, string resolution) {
        string params = "/scan/support-resistance?symbol=" + ticker + "&resolution=" + resolution;
        json::value retVal = Connect::getJson(finnhubUrl, params, finnhubToken);
        
        vector<double> res;

        if (retVal[U("levels")].is_null()) {
            CPPFINANCIALDATA_WARN("There is no support/resistance data for {}", ticker);
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

        if (retVal.at("technicalAnalysis").at("signal").is_null()) CPPFINANCIALDATA_WARN("There is no aggregate data for {}", ticker);

        string signal = retVal.at("technicalAnalysis").at("signal").as_string();
        double adx = retVal.at("trend").at("adx").as_double();
        bool trending = retVal.at("trend").at("trending").as_bool();

        AggregateData res(signal, adx, trending);
        return res;
    }
}