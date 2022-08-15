#ifndef MACRODATA_H
#define MACRODATA_H

#include "FormatRequest.h"

namespace MacroData
{
    struct EconomicEvent {
        string event;
        string date;
        string country;
        long unixTime;
        double actual = 0;
        double previous = 0;
        double change = 0;
        double changePct = 0;
        double estimate = 0;
        string impact;
};

    struct TreasuryRates {
        string date;
        long unixDate;
        float oneMonth;
        float twoMonth;
        float threeMonth;
        float oneYear;
        float twoYear;
        float fiveYear;
        float sevenYear;
        float tenYear;
        float twentyYear;
        float thirtyYear;
    };

    struct EconomicIndicator {
        string attribute;
        string date;
        long unixDate;
        double value;
    };

    // No date inputs only gives future economic events
    vector<EconomicEvent> getEconomicCalendar(string from = "", string to = "");
    // empty from will return last 30 days, data available 3 months at a time
    vector<TreasuryRates> getTreasuryRates(string from = "", string to = "");
    // Up to 50 years of data, default is monthly iterval with 10yr
    vector<EconomicIndicator> getTreasuryHistory(string maturity = "", string interval = "");
    
    // Up to 50 years of historical economic data, some quarterly, some monthly, some annual
    vector<EconomicIndicator> getEconomicIndicator(string indicator, string from, string to = "");




    // Using global variables for indicators to add simplicity
    extern string GrossDomesticProduct;
    extern string realGDP;
    extern string realGDPperCapita;
    extern string federalFunds;
    extern string ConsumerPriceIndex;
    extern string inflationRate; 
    extern string inflation;
    extern string retailSales;
    extern string consumerSentiment;
    extern string durableGoods;
    extern string unemploymentRate;
    extern string totalNonfarmPayroll;
    extern string initialClaims;
    extern string industrialProductionTotalIndex;
    extern string newPrivatelyOwnedHousingUnitsStartedTotalUnits;
    extern string totalVehicleSales;
    extern string retailMoneyFunds;
    extern string smoothedUSRecessionProbabilities;
    extern string threeMoRatesAndYieldsCertificatesOfDeposit;
    extern string commercialBankCreditCardInterestRate;
    extern string thirtyYrFixedRateMortgageAvg;
    extern string fifteenYrFixedRateMortgageAvg;
}

#endif // MACRODATA_H