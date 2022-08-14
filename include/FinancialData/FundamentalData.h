#ifndef FUNDAMENTALDATA_H
#define FUNDAMENTALDATA_H

#include "FormatRequest.h"

namespace Fundamentals 
{
    struct Earnings {
        string date;
        long unixTime;
        double epsEstimate;
        double epsActual = 0;
        int quarter = 0;
        int timeCode;
        int year = 0;
    };
    
    struct SupplyChainRelations {
        string ticker;
        bool customer;
        bool supplier;
        string relatedSymbol;
        float twoWkCorrelation;
        float oneMonthCorrelation;
        float oneYrCorrelation;
    };

    struct FinancialScores {
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

    struct CompanyProfile {
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

    struct InsiderTrade {
        string symbol;
        string filingDate;
        long filingDateUnix;
        string transactionDate;
        long txnDateUnix;
        string transactionType;
        bool isSale = false;
        long securitiesTransacted;
        double price;
        double totalTransaction;
    };

    // Can send a request for a single metric if necessary, most of these attributes can be found in other functions
    json::value getFinancialData(const string ticker, string metric = "");

    // Get the nearest earnings event for a specific stock
    Earnings earningsUpcoming(const string ticker);

    // Historical earnings, limit is each earning event, ie 4 per fiscal year
    vector<Earnings> earningsHistorical(const string ticker, string limit = "1");

    // List of companies suppliers and customers
    vector<SupplyChainRelations> supplyChainData(const string ticker);

    // See Financial scores type for all attributes
    FinancialScores getFinancialScores(const string ticker);
    vector<FinancialScores> getQuarterlyFinancialScores(const string ticker, string limit = "1");
    
    // Company profile contains many more attributes than the current specified, may need to adjust if other data provides to be useful
    CompanyProfile getCompanyProfile(const string ticker);

    // Note most stocks do not insider trade as frequently as companies like apple, this data appears to reach back pretty far
    vector<InsiderTrade> getCompanyInsiderTrades(const string ticker, string limit = "1");
}

#endif // FUNDAMENTALDATA_H