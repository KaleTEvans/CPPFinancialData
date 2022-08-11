#ifndef NEWSDATA_H
#define NEWSDATA_H

#include "FormatRequest.h"

namespace News
{
    unordered_map<string, double> newsSentiment(const string ticker);
}

#endif // NEWSDATA_H