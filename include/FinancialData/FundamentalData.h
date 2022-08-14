#ifndef FUNDAMENTALDATA_H
#define FUNDAMENTALDATA_H

#include "FormatRequest.h"

namespace Fundamentals 
{
    class Earnings {
        public:
            string date;
            long unixTime;
            double epsEstimate;
            double epsActual = 0;
            int quarter = 0;
            int timeCode;
            int year = 0;
    };
    
    class SupplyChainRelations {
        public:
            string ticker;
            bool customer;
            bool supplier;
            string relatedSymbol;
            float twoWkCorrelation;
            float oneMonthCorrelation;
            float oneYrCorrelation;
    };

    class FinancialScores {
        public:
            double peRatio;
            double pegRatio; // lower is better
            double debtRatio; // total liabilities/total assets
            double debtEquityRatio;
            double priceFairValue;
            double netProfitMargin;
            double piotroskiScore = 0; // 0-9
            string date = "";
            long unixDate = 0;
    };

    class CompanyProfile {
        public:
            string symbol;
            double beta;
            double volAvg;
            double marketCap;
            string cik;
            string isin;
            string exchange;
            string industry;
            string sector;
            vector<string> peers;
    };

    json::value getFinancialData(const string ticker, string metric = "");
    Earnings earningsUpcoming(const string ticker);
    vector<Earnings> earningsHistorical(const string ticker, string limit = "1");
    vector<SupplyChainRelations> supplyChainData(const string ticker);
    FinancialScores getFinancialScores(const string ticker);
    vector<FinancialScores> getQuarterlyFinancialScores(const string ticker, string limit = "1");
    CompanyProfile getCompanyProfile(const string ticker);
}

#endif // FUNDAMENTALDATA_H