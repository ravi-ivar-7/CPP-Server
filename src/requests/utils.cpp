#include <boost/algorithm/string.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/asio.hpp>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <sstream>


namespace beast = boost::beast;
namespace http = beast::http;
namespace asio = boost::asio;
using tcp = asio::ip::tcp;


std::unordered_map<std::string, std::string> getQueryParams(const std::string &query)
{
    std::unordered_map<std::string, std::string> keyValues;
    std::vector<std::string> pairs;
    boost::algorithm::split(pairs, query, boost::is_any_of("&"));
    for (const auto &pair : pairs)
    {
        std::vector<std::string> thisKeyValue;
        boost::algorithm::split(thisKeyValue, pair, boost::is_any_of("="));
        if (thisKeyValue.size() == 2)
        {
            keyValues[thisKeyValue[0]] = thisKeyValue[1];
        }
    }
    return keyValues;
}
