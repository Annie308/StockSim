#pragma once

#include <portfolio.h>
#include <input.h>
#include <utils.h>
#include <map>
#include <iostream>

extern double starting_balance;
std::tuple<double,double,double> CalucateBalance();
std::map<std::string,double> update_balance(double start_bal);
void init_balance();
void clear_balance();
double read_balance();
void update_balance_file();
void write_to_balance(double &avail_balance, double &original_balance, double &current_balance);
