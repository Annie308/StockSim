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
    string symbol;
    buy();
    view_portfolio();
    edit_portfolio();
    system("start portfolio.csv");
    system("start balance.csv");
    return 0;
}