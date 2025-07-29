//API Key: 0H5JGZYVZZ9SGXQB
//API Key: 24UILB7V5QX3KJOK

#pragma once

#include <iostream>
#include <nlohmann/json.hpp>
#include <cpr/cpr.h>

using nlohmann::json;

struct StockInfo{
    std::string info;
    std::string time;
    double ask;
    double bid;
    double bid_size;
    int volume;
};

extern StockInfo myStockInfo;

bool fetch_stock_data(const std::string &input_symbol);