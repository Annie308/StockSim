#pragma once

#include<iostream>
#include <portfolio.h>
#include <utils.h>
#include <limits>
#include <optional>

enum class Options {
    BUY,
    SELL
};

std::optional<Options> int_to_options(int input);
Options buy_or_sell();
int choose_stock(int &max_stock_num);
int shares_num(int &max_shares_num);
bool edit();