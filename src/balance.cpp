#include "portfolio.h"
#include <map>
#include "balance.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <tuple>

using namespace std;


std::map<std::string, double> balance;
string file_name = "balance.csv";
double starting_balance =40000;

tuple<double,double,double> CalucateBalance(){
    double buy_price=0;
    double curr_price =0;
    double cash_spent =0;
    double total_profit=0;
    for (int i=0; i<purchases.size(); i++){
        buy_price += purchases[i].buying_price * purchases[i].shares;
        curr_price += purchases[i].selling_price * purchases[i].shares;
        total_profit += purchases[i].capital_gain;
    }

    return {buy_price, curr_price, total_profit};
}

std::map<string,double> update_balance(double start_bal){
    tuple<double, double,double> result = CalucateBalance();
    balance["Original portfolio balance"] = get<0>(result);
    balance["Current portfolio balance"] = get<1>(result);
    balance["Total portfolio profit"] = get<2>(result);
    balance["Cash available"] = start_bal - target_stock_price * target_stock_shares_num;
    cout <<"balance updated! "<<balance["Cash available"]<<endl;
    return balance;
}

void init_balance(){
    std::ifstream infile(file_name);
    if (!infile.is_open() || infile.peek() == std::ifstream::traits_type::eof()){
        std::ofstream outfile(file_name);
        outfile <<"Cash availible,Original Portfolio Balance,Current Portfolio Balance,Total portfolio profit\n";                                                               //Writing header
        outfile.close();
    }
    infile.close();
}

void clear_balance(){
    std::ofstream file(file_name, ios::trunc);
    file.close();
}

double read_balance(){
    std::ifstream file(file_name);
    string line;
    bool remove_header = true;

    while (getline(file, line)){
        std::stringstream ss(line);
        string str_balance;
        string str_profit;
        if (remove_header){
            for (int i=0; i<4; i++){   
                string token; 
                getline(ss, token, ',');        //remove header
            }
            remove_header = false;
        }
        getline(ss, str_balance, ',');
        getline(ss, str_profit, ',');
        if (!str_balance.empty()&&!str_profit.empty()){
            starting_balance = stof(str_balance);
            balance["Total portfolio profit"] = stoi(str_profit);
        }
        
    }
    return starting_balance;
}


void write_to_balance(double &avail_balance, double &original_balance, double &current_balance, double &total_profit){
    clear_balance();
    init_balance();
    std::ofstream file(file_name, ios::app);
    if (!file.is_open()){
        cerr << "Cannot open file to update."<<endl;
        return;
    }
    file << avail_balance<< ","
        << original_balance<<","
        << current_balance<<","
        <<total_profit<<"\n";
    file.close();
}


void update_balance_file(){
    init_balance();
    double new_start_bal = read_balance();     //gets starting balance
    std::map new_balance = update_balance(new_start_bal);   //calculates new balance

    double avail_balance = new_balance["Cash available"];
    double original_balance = new_balance["Original portfolio balance"];
    double current_balance = new_balance["Current portfolio balance"];
    double total_profit = new_balance["Total portfolio profit"];

    write_to_balance(avail_balance, original_balance, current_balance, total_profit);
}

