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
        outfile <<"Nr,Date,Transaction Type,Shares,Ticker,Buying Price,Selling Price,Bid Price,Ask Price,Current Value,Cap. Gain,Per. Gain\n";
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
        purchase.current_ask = myStockInfo.ask;
        purchase.current_bid = myStockInfo.bid;
        cout <<purchase.nr<<"  "<<purchase.date<<"  "<<purchase.transaction_type<<"  "<<purchase.shares<<"  "<<purchase.ticker<<"  "<<purchase.buying_price<<"   "<<purchase.selling_price<<"   "<<purchase.current_ask<<"   "<<purchase.current_ask<<"  "<<purchase.current_value<<"   "<<purchase.capital_gain<<"   "<<purchase.percent_gain<<endl;
    }
}

void add_to_portfolio(){
    cout <<"updating..."<<endl;
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
            <<std::fixed<< std::setprecision(2)<< purchase.current_bid <<","
            <<std::fixed<< std::setprecision(2)<< purchase.current_ask <<","
            <<std::fixed<< std::setprecision(2)<< purchase.current_value <<","
            <<std::fixed<< std::setprecision(2)<< purchase.capital_gain <<","
            <<std::fixed<< std::setprecision(2)<<purchase.percent_gain<<"\n";
    }
    file.flush();
    file.close();
}

void buy(){  
    string stock;

    do{
        cout << "Which stock to buy? ";
        cin >> stock;
        transform(stock.begin(), stock.end(), stock.begin(), ::toupper);        //make uppercase
    }
    while(!fetch_stock_data(stock));

    double avail_cash = read_balance();         
    cout << "\nCash available: "<< avail_cash<<endl;
    cout << "Buying: " << myStockInfo.info << "\nAsk Price: " << myStockInfo.ask <<endl;

    int max_shares = floor(avail_cash/myStockInfo.ask);         //find max shares that can be purchased  
    int num = shares_num(max_shares);                           //gets user input for how many shares to buy

    StockPurchase p;
    p.nr = getCol();
    p.date = myStockInfo.time;
    p.transaction_type = "Buy";
    p.shares = num;
    p.ticker = myStockInfo.info;
    p.buying_price = myStockInfo.ask;
    p.selling_price = 0.00;
    p.current_bid = myStockInfo.bid;
    p.current_ask = myStockInfo.ask;
    p.current_value = 0.00;
    p.capital_gain = 0.00;
    p.percent_gain = 0.00;
    purchases.push_back(p);

    target_stock_price = p.buying_price;
    target_stock_shares_num = num;
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
            string str_current_bid;
            string str_current_ask;
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
            getline(ss, str_current_bid, ',');
            getline(ss, str_current_ask, ',');
            getline(ss, str_current_value, ',');
            getline(ss, str_capital_gain, ',');
            getline(ss, str_percent_gain, ',');
            if (!str_nr.empty()&& !str_buying_price.empty() && !str_shares.empty() && !str_selling_price.empty() &&!str_current_bid.empty() && !str_current_ask.empty() &&!str_capital_gain.empty() &&!str_current_value.empty()&&!str_percent_gain.empty()){
                p.nr = stoi(str_nr);
                p.shares = stoi(str_shares);
                p.buying_price = stof(str_buying_price);
                p.selling_price = stof(str_selling_price);
                p.current_bid = stof(str_current_bid);
                p.current_ask = stof(str_current_ask);
                p.current_value = stof(str_current_value);
                p.capital_gain = stof(str_capital_gain);
                p.percent_gain = stof(str_percent_gain);
            }
            purchases.push_back(p);
        }
        
    }
}

void edit_portfolio(){
    int max_stock_num = getCol();
    int choice = choose_stock(max_stock_num);
    cout <<purchases[choice-1].nr<< " " << purchases[choice-1].date <<" " <<purchases[choice-1].transaction_type << " " << purchases[choice-1].shares << " "<< purchases[choice-1].ticker << " "<< purchases[choice-1].buying_price << " "<< purchases[choice-1].selling_price<< " "<<" "<<purchases[choice-1].current_bid <<"  "<<purchases[choice-1].current_ask<<"  "<<purchases[choice-1].percent_gain<<endl;

    StockPurchase p;                                //new stock   
    p = purchases[choice-1];        //copy the stock to edit
    p.nr = getCol();                    //put in the end
    p.date = myStockInfo.time;        //update date

    if (choice <= purchases.size()){      
        Options userOpt = buy_or_sell();

        if (userOpt == Options::BUY){
            p.shares = shares_num(purchases[choice-1].shares);   //getting how many stocks to buy
            p.transaction_type = "Buy";        
            p.buying_price = p.current_ask;        //buying price = bidding price
            cout <<"Bought "<<p.shares<<" shares. You now have: "<<p.shares<<" shares of "<<p.ticker<<" at $"<<p.current_bid<<" per share.\n";
            target_stock_shares_num = p.shares;      //update target stock shares num
        }
        else if (userOpt == Options::SELL){
            p.shares = shares_num(purchases[choice-1].shares);           //getting how many stocks to sell
            p.transaction_type = "Sell";
            p.selling_price = p.current_bid;        //selling price = bidding price
            cout <<"Sold "<<p.shares<<" shares. You now have: "<<p.shares<<" shares of "<<p.ticker<<" at $"<<p.current_bid<<" per share.\n";
            target_stock_shares_num = -p.shares;    

            p.percent_gain = (p.selling_price/p.buying_price) -1;       //updates profit/loss
            p.capital_gain = (p.selling_price-p.buying_price)*p.shares;
            p.current_value = p.selling_price*p.shares;
        }
    }
    target_stock_price = p.current_bid;
    purchases.push_back(p);        //add new stock to purchases
}

void init_portfolio(){
    init_file();
    view_portfolio();
    update_stock();

    Edit Opt = edit_or_buy();

    if (Opt==Edit::BUY){      
        buy();
    }else if (Opt==Edit::EDIT) {
        if (purchases.empty()){
            cout << "No stocks to edit. Please buy a stock first.\n";
            buy();
            return;
        }
        edit_portfolio();
    }
}

