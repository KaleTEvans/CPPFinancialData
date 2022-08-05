
// Use for future reference with earnings


// vector<unordered_map<string, double>> earningsUpcoming(string start, string endDate, string ticker) {
//         string params = "/calendar/earnings?from=" + start + "&to=" + endDate + "&symbol=" + ticker;
//         json::value retVal = getJson(base, params, end);

//         vector<unordered_map<string, double>> res;

//         auto earningsArr = retVal.at(U("earningsCalendar")).as_array();
//         if (earningsArr.size() < 1) std::cout << "COCK1" << std::endl;
//         for (auto it = earningsArr.begin(); it != earningsArr.end(); ++it) {
//             auto& data = *it;
//             json::value dataObj = data;
//             // get values to calculate percent difference
//             double epsActual = dataObj[U("epsActual")].as_double();
//             double epsEstimate = dataObj[U("epsEstimate")].as_double();
//             double percentOff = (epsActual - epsEstimate) / epsEstimate;
//             // 1 : before open, 2 : after close, 3 : during 
//             auto hour = dataObj[U("hour")].as_string();
//             int timeCode;
//             if (hour == "bmo") timeCode = 1;
//             else if (hour == "amc") timeCode = 2;
//             else if (hour == "dmh") timeCode = 3;
//             else timeCode = 0;

//             unordered_map<string, double> m;
//             m.insert({"epsactual", epsActual});
//             m.insert({"epsestimate", epsEstimate});
//             m.insert({"pctoff", percentOff});
//             m.insert({"timereleased", timeCode});
//             m.insert({"quarter", dataObj[U("quarter")].as_double()});
//             m.insert({"year", dataObj[U("year")].as_double()});

//             res.push_back(m);
//         }

//         return res;
//     }