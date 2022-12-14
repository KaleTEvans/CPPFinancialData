#include "FinancialData/FormatRequest.h"

namespace Connect 
{
    json::value getJson(string base, string link, string end) {
        json::value retVal;

        auto requestJson = http_client(_XPLATSTR(base))
            .request(methods::GET,
                uri_builder(_XPLATSTR(link + end)).to_string())
        // Get the response
        .then([](http_response response) {
            // Check the status code.
            if (response.status_code() != 200) {
                throw std::runtime_error("Returned " + std::to_string(response.status_code()));
            }

            // Convert the response body to JSON object.
            return response.extract_json();
        })
        // get the data field
        .then([&](json::value jsonObject) {
            retVal = jsonObject;
        });

        // Wait for the concurrent tasks to finish.
        try {
            requestJson.wait();
        } catch (const std::exception &e) {
            CPPFINANCIALDATA_FATAL("Error making request: {}", e.what());
        }

        return retVal;
    }
}