#ifndef FORMATREQUEST_H
#define FORMATREQUEST_H

#include <iostream>
#include <ctime>
#include <unordered_map>
#include <vector>
#include <cpprest/http_client.h>
#include <cpprest/filestream.h>
#include <cpprest/uri.h>
#include <cpprest/json.h>
#include <cpprest/ws_client.h>

using namespace utility;
using namespace web;
using namespace pplx;
using namespace concurrency::streams;
using namespace web::websockets::client;
using namespace utility::conversions;
using namespace web::http;
using namespace web::http::client;

using std::string;
using std::unordered_map;
using std::vector;

namespace Connect 
{
    json::value getJson(string base, string link, string end);
}

#endif // FORMATREQUEST_H