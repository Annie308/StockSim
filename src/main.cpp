#include <iostream>
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include "stock_data.h"
#include "portfolio.h"
#include "input.h"
#include "balance.h"
#include "utils.h"
#include "input.h"

using namespace std;
using nlohmann::json;

int main(){
    init_portfolio();
    clear_file();
    init_file();
    add_to_portfolio();
    update_balance_file();
    system("start portfolio.csv");
    system("start balance.csv");
    return 0;
}