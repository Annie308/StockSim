#include "stock_data.h"
#include "balance.h"
#include "portfolio.h"
#include <iostream>
#include <nlohmann/json.hpp>
#include <cpr/cpr.h>

using namespace std;
using nlohmann::json;

struct StockInfo;

StockInfo myStockInfo;

void fetch_stock_data(const string &input_symbol){
std::string symbol = input_symbol;
    std::string url = "https://data.alpaca.markets/v2/stocks/" + symbol + "/quotes/latest";

    auto response = cpr::Get(
        cpr::Url{url},
        cpr::Header{
            {"APCA-API-KEY-ID", "PKRC8BNLX7N2LWF4S145"},
            {"APCA-API-SECRET-KEY", "Z5req6Hh4exF8WdiXOTQMwloG5mrjF9JbbUufzUk"}
        }
    );

    std::cout << "Status Code: " << response.status_code << std::endl;
    
    if (response.status_code == 200) {
        nlohmann::json result = nlohmann::json::parse(response.text);
            myStockInfo.info = result["symbol"];
            myStockInfo.time = result["quote"]["t"];
            myStockInfo.ask = result["quote"]["ap"];
            myStockInfo.bid = result["quote"]["bp"];
            myStockInfo.bid_size = result["quote"]["bs"];
    } else {
        cerr << "Failed to fetch stock data." << endl;
    }
}