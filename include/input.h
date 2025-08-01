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

enum class Edit {
    BUY,
    EDIT
};

std::optional<Options> int_to_options(int input);
Options buy_or_sell();
Edit edit_or_buy();
int choose_stock(int &max_stock_num);
int shares_num(int &max_shares_num);