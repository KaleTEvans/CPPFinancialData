# CPPFinancialData

C++ Library with the purpose of simplifying the acquisistion of financial data from various web apis

## Install

Note that in order to use this library you must provide your own API keys

```console
$ git clone https://github.com/KaleTEvans/CPPFinancialData.git
$ cd CPPFinancialData && mkdir build && cd build
$ cmake .. && make 
```

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
PriceData::FSocket* socket = new PriceData::FSocket(std::vector<std::string>& tickers);
socket->openSocket() // Adds tickers to queue and calls runSocket(), which opens up the socket connection
// Run for specified amount of time
delete socket; // Calls closeSocket() and ends socket connection
```

## Finnhub Connections

### PriceData namespace
* Get various data points for a single ticker

```c++
unordered_map<string, double> PriceData::getQuote(string ticker);
// Returns the following values:
"current" // current price
"pctchange" // percent change
"highofday" // high price of day
"lowofday" // low price of day
"open" // opening price
"prevclose" // close from previous day  

unordered_map<string, double> PriceData::latestBidAsk(string ticker);
// Returns the following values
"ask"
"askvol"
"bid"
"bidvol"
"time" // time in unix
```

### Fundamentals namespace
* Get fundamental data about a certain stock

```c++
unordered_map<string, double> Fundamentals::NewsSentiment(string ticker);
// Returns various news sentiment scores and traffic
"articlesinlastweek" 
"buzz" // score for amount of current news traffic
"weeklyaverage"
"companynewsscore" // NOTE: All scores are based from 0-1
"sectoravgbullishpct"
"sectoravgnewsscore"
"bearsentiment"
"bullsentiment"

json::value Fundamentals::getFinancialData(string ticker, string metric);
// This is left as a json object because there are hundreds of metrics to choose from 

unordered_map<string, double> Fundamentals::earningsUpcoming(string ticker) ;
// Returns the closest upcoming earnings data
"date" // YYYY-MM-DD
"quarter"
"epsestimate"
"timecode"
"year"

std::pair<long, int> Fundamentals::twitterMentions(string ticker);
// Returns the unix stamp and number of tweets about the specific ticker within the last hour

vector<SupplyChainRelations*> Fundamentals::supplyChainData(string ticker);
// Returns suppliers and customers for a certain company
// View the FinnhubConnections.h to see what data points are contained in the SupplyChainRelations type
```

### TechnicalData namespace
* Get technical indicators for a given ticker and time resolution (1, 5, 15, 30, 60, D, W, M)
* NOTE: Default resolution is one day (D)

```c++
 vector<ChartPatternData*> getChartPatterns(const string ticker, string resolution);
 // Returns an array of chart pattern data types
 // These are all public data members
 string patternName; // Get the name of the current chart pattern
 string patternType; // Bullish or Bearish
 string status; // Complete or Incomplete
 vector<pair<string, float>> pricePoints; // Contains the price points and unix times that form the pattern

 vector<double> getSupportAndResistance(string ticker, string resolution);
 // Returns a vector of support and resistance prices

 AggregateData getAggregateIndicators(string ticker, string resolution);
 // Returns AggregateData type containing three public members
 string signal; // Bullish, bearish, or neutral
 double adx; // ADX reading
 bool trend; // Whether the ADX indicates the stock is trending
```