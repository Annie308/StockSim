#include <iostream>
#include <fstream>
#include <iomanip>
#include <map>
#include <string>
#include <sstream>
#include <filesystem>
#include <cctype>
#include <cmath>
#include <algorithm>
#include "input.h"
#include "portfolio.h"
#include "balance.h"
#include "stock_data.h"

using namespace std;

std::vector<StockPurchase> purchases;
int target_stock_shares_num;
double target_stock_price;

void init_file(){
    std::ifstream infile("portfolio.csv");
    if (!infile.is_open() || infile.peek() == std::ifstream::traits_type::eof()){
        std::ofstream outfile("portfolio.csv");
        outfile <<"Nr,Date,Transaction Type,Nr of Shares,Ticker,Buying Price Per Share,Selling Price Per Share,Current Price Per Share,Current Value,Capital Gain,Percent Gain\n";
        outfile.close();
    }
    
    infile.close();
}

void clear_file(){
    std::ofstream file("portfolio.csv", std::ios::trunc);
    file.flush();
    file.close();
}

int getCol(){
    std::ifstream infile("portfolio.csv");
    string line;
    string token;
    int col = 0;

    if (!infile.is_open() || infile.peek() == std::ifstream::traits_type::eof()){
        col = 1;
    }
    while(getline(infile, line)){
        std::stringstream ss(line);
        if (getline(ss, token, ',')){
            col ++;
        }
    }
    return col;
}

void update_stock(){
    for (auto &purchase:purchases){
        fetch_stock_data(purchase.ticker);              //gets current price of stock 
        purchase.current_price = myStockInfo.ask;
        double sell_price = purchase.current_price;
        double buy_price = purchase.buying_price;
        purchase.percent_gain = (sell_price/buy_price) -1;       //updates profit/loss
        purchase.capital_gain = (sell_price-buy_price)*purchase.shares;
        purchase.current_value = sell_price*purchase.shares;
        cout <<purchase.nr<< " " << purchase.date <<" " <<purchase.transaction_type << " " << purchase.shares << " "<< purchase.ticker << " "<< purchase.buying_price << " "<< purchase.selling_price<< " "<<" "<<purchase.current_price <<" " <<purchase.percent_gain<<endl;
    }
}

void add_to_portfolio(){
    std::ofstream file("portfolio.csv", ios::app);

    if (!file.is_open()){
        cerr<< "File did not open!";
        return;
    }

    for (auto& purchase: purchases){
        file <<purchase.nr << ","
            <<purchase.date << ","
            <<purchase.transaction_type << ","
            <<purchase.shares << ","
            <<purchase.ticker << ","
            <<std::fixed<< std::setprecision(2)<< purchase.buying_price <<","
            <<std::fixed<< std::setprecision(2)<< purchase.selling_price <<","
            <<std::fixed<< std::setprecision(2)<< purchase.current_price <<","
            <<std::fixed<< std::setprecision(2)<< purchase.current_value <<","
            <<std::fixed<< std::setprecision(2)<< purchase.capital_gain <<","
            <<std::fixed<< std::setprecision(2)<<purchase.percent_gain<<"\n";
    }
    file.flush();
    file.close();
}

void buy(){
    string stock;
    cout << "Which stock to buy? ";
    cin >> stock;
    transform(stock.begin(), stock.end(), stock.begin(), ::toupper);        //make uppercase
    fetch_stock_data(stock);
    cout << "Buying: " << myStockInfo.info << "\nAsk Price: " << myStockInfo.ask <<endl;

    init_balance();
    double avail_cash = read_balance();
    cout << "\nCash available: "<< avail_cash<<endl;

    int max_shares = floor(avail_cash/myStockInfo.ask);       
    int shares = shares_num(max_shares);        

    StockPurchase p;
    p.nr = getCol();
    p.date = myStockInfo.time;
    p.transaction_type = "Buy";
    p.shares = shares;
    p.ticker = myStockInfo.info;
    p.buying_price = myStockInfo.ask;
    p.selling_price = myStockInfo.bid;
    p.current_price = myStockInfo.ask;
    p.current_value = 0.00;
    p.capital_gain = 0.00;
    p.percent_gain = 0.00;
    purchases.push_back(p);

    target_stock_price = p.current_price;
    target_stock_shares_num = shares;
    init_file();
    add_to_portfolio();
    update_balance_file();
}

void view_portfolio(){
    std::ifstream file("portfolio.csv");
    string line;
    bool remove_header = true;
    StockPurchase p;
    purchases.clear();

    while (getline(file, line)){
        std::stringstream ss(line);     //split the line by commas
                                        //creates a stringstream object that can be used like cin
        
        if (remove_header){
            for (int i=0; i <HEADER_COUNT; i++){
                string token;
                getline(ss, token, ',');
                remove_header = false;
                cout <<token <<" ";
            }
            cout <<"\n";
        }else{
            string str_nr;
            string str_shares;
            string str_buying_price;
            string str_selling_price;
            string str_current_price;
            string str_current_value;
            string str_capital_gain;
            string str_percent_gain;
            getline(ss, str_nr, ',');
            getline(ss, p.date, ',');
            getline(ss, p.transaction_type, ',');
            getline(ss, str_shares, ',');
            getline(ss, p.ticker, ',');
            getline(ss, str_buying_price, ',');
            getline(ss, str_selling_price, ',');
            getline(ss, str_current_price, ',');
            getline(ss, str_current_value, ',');
            getline(ss, str_capital_gain, ',');
            getline(ss, str_percent_gain, ',');
            if (!str_nr.empty()&& !str_buying_price.empty() && !str_shares.empty() && !str_selling_price.empty() &&!str_current_price.empty() && !str_capital_gain.empty() &&!str_current_value.empty()&&!str_percent_gain.empty()){
                p.nr = stoi(str_nr);
                p.shares = stoi(str_shares);
                p.buying_price = stof(str_buying_price);
                p.selling_price = stof(str_selling_price);
                p.current_price = stof(str_current_price);
                p.current_value = stof(str_current_value);
                p.capital_gain = stof(str_capital_gain);
                p.percent_gain = stof(str_percent_gain);
            }
            purchases.push_back(p);
        }
        
    }
    update_stock();


}

void edit_portfolio(){
    int max_stock_num = getCol();
    int choice = choose_stock(max_stock_num);

    init_balance();
    double avail_cash = read_balance();
    cout << "\nCash available: "<< avail_cash<<endl;

    if (choice <= purchases.size()){        
        Options userOpt = buy_or_sell();

        purchases[choice-1].date = get_current_time();
        double curr_price = purchases[choice-1].current_price;
        double bid_price = purchases[choice-1].selling_price;

        if (userOpt == Options::BUY){
            double avail_cash = read_balance();         
            cout << "\nCash available: "<< avail_cash<<endl;

            int max_shares = floor(avail_cash/myStockInfo.ask);         //find max shares that can be purchased  
            int num = shares_num(max_shares);  

            purchases[choice-1].shares += num;
            purchases[choice-1].transaction_type = "Buy";
            purchases[choice-1].buying_price = curr_price;
            cout <<"Aquried "<<num<<" more shares. You now have: "<<purchases[choice-1].shares<<" shares of "<<purchases[choice-1].ticker<<" at $"<<purchases[choice-1].current_price<<" per share.\n";
            target_stock_shares_num = num;
        }
        else if (userOpt == Options::SELL){
            int num = shares_num(purchases[choice-1].shares);
            while (!validate_bounds(num, purchases[choice-1].shares)){
                cout <<"You don't have enough shares!";
                num = shares_num(purchases[choice-1].shares);
            }
            purchases[choice-1].shares -= num;
            purchases[choice-1].transaction_type = "Sell";
            purchases[choice-1].selling_price = bid_price;
            cout <<"Sold "<<num<<" shares. You now have: "<<purchases[choice-1].shares<<" shares of "<<purchases[choice-1].ticker<<" at $"<<purchases[choice-1].current_price<<" per share.\n";
            target_stock_shares_num = -num;    
    
        }

        }
    target_stock_price = purchases[choice-1].current_price;
    
    update_stock();
    clear_file();
    init_file();
    add_to_portfolio();
    update_balance_file();
}

