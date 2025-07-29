#pragma once

#include <iostream>
#include <string>
#include <vector>

struct StockPurchase {
    int nr;
    std::string date;
    std::string transaction_type;
    int shares;
    std::string ticker;
    double buying_price;
    double selling_price;
    double current_price;
    double current_value;
    double capital_gain;
    double percent_gain;
};

const int HEADER_COUNT = 11;

extern std::vector<StockPurchase> purchases;
extern int target_stock_shares_num;
extern double target_stock_price;

void init_file();
void clear_file();
void add_to_portfolio();
int getCol();
void buy();
void edit_portfolio();
void view_portfolio();
