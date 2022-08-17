# CPPFinancialData

C++ Library with the purpose of simplifying the acquisistion of financial data from various web apis

## Install

Note that in order to use this library you must provide your own API keys

```console
$ git clone https://github.com/KaleTEvans/CPPFinancialData.git
$ cd CPPFinancialData && mkdir build && cd build
$ cmake .. && make 
```

## Required Libraries
* cpprestsdk
```sudo apt-get install libcpprest-dev```

* spdlog
```sudo apt install libspdlog-dev```

## Platforms 
* Linux

## Features
* Financial data from Finnhub

In the future, planned additions are:
* AlphaVantage historical data
* IEX

## Current Functions

### Finnhub Socket
* Stream current price data for up to 10 different tickers

```c++
FSocket* socket = new FSocket(std::vector<std::string>& tickers)

socket->openSocket() // Adds tickers to queue and calls runSocket(), which opens up the socket connection until destructor is called

delete socket; // Calls closeSocket() and ends socket connection
```

## Data Methods

### PriceData namespace
* Get various data points for a single ticker

```c++
Quote getQuote(string ticker)

BidAsk latestBidAsk(string ticker)

vector<Candle> getHistoricalPrice(string ticker, string interval) // Gives price data for up to about two months, depending on interval
```

### Fundamentals namespace
* Get fundamental data about a certain stock
* NOTE: Default limit is 1

```c++
json::value getFinancialData(string ticker, string metric) // Many different metrics, see finnhub api docs

Earnings earningsUpcoming(string ticker) // Nearest upcoming earnings date and estimates

vector<Earnings> earningsHistorical(cstring ticker, string limit) // Get list of all historical earnings 

vector<SupplyChainRelations> supplyChainData(string ticker) // Suppliers and customers for a certain company and their level of correlation

FinancialScores getFinancialScores(string ticker) // Get financial scores, such as P/E, debt ratio, etc

vector<FinancialScores> getQuarterlyFinancialScores(string ticker, string limit)
CompanyProfile getCompanyProfile(string ticker) // Attributes such as beta, average volume, market cap, etc

vector<InsiderTrades> getCompanyInsiderTrades(string ticker, string limit) // List of all insider transactions and dates
```

### TechnicalData namespace
* Get technical indicators for a given ticker and time resolution (1, 5, 15, 30, 60, D, W, M)
* NOTE: Default resolution is one day (D)

```c++
vector<double> getSupportAndResistance(string ticker, string resolution); 
AggregateData getAggregateIndicators(string ticker, string resolution); // Gathers signals from various technical indicators and provides a summary

```

### News namespace
* Get news information, such as sentiment, historical articles, with detailed descriptions
* NOTE: Default limit is 1, Default page is 0

```C++
NewsSentiment newsSentiment(string ticker) // Average news articles, bullish / bearish, sector averages, etc
vector<NewsArticle> getPressReleases(string ticker, string limit) // Data goes back to about 2019
vector<NewsArticle> getSingleStockNews(string ticker, string limit) // Same data as press releases but not as tailored to a single ticker
vector<SocialSentiment> getSocialSentiment(string ticker, string page) // 100 per page, data from stocktwits and twitter
```

### MacroData namespace
* Macroeconomic data with about 50 years of historical data
* NOTE: Default 'to' date is your current local date

```C++
vector<EconomicEvent> getEconomicCalendar(string from, string to) // Get economic calendar, filters only EU and US events with an impact of medium-high
vector<TreasuryRates> getTreasuryRates(string from, string to) // Default will give last 30 days, maximum is 3 months of data at a time, these are updated daily
vector<EconomicIndicator> getTreasuryHistory(string maturity, string interval) // Available intervals are 3month, 2year, 5year, 7year, 10year, and 30year
vector<EconomicIndicator> getEconomicIndicator(string indicator, string from, string to) // Various data, listed in header file
```

### MarketData namespace
* Sector data and active stocks

```C++
vector<Sector> getSingleSectorMetrics(string date) // Array contains each ector current P/E, daily data
vector<Sector> getHistoricalSectorPctChange(string sector, string limit)
vector<Sector> getCurrentSectorChange() // Percent change for each sector, updated hourly
vector<NotableStock> getMostActiveStocks()
```