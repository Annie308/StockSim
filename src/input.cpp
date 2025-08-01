#include<iostream>
#include "portfolio.h"
#include <limits>
#include <optional>
#include "input.h"

using namespace std;

std::optional<Options> int_to_options(int input){
    if (input == 1){
        return Options::BUY;
    }else if (input == 2){
        return Options::SELL;
    }else{
        return nullopt;
    }
}

std::optional<Edit> int_to_edit(int input){
    if (input == 1){
        return Edit::BUY;
    }else if (input == 2){
        return Edit::EDIT;
    }else{
        return nullopt;
    }
}

Options buy_or_sell(){
    int input;
    std::optional<Options> inputOpt;

   do {
        cout << "1. Buy\n2. Sell\n";
        if (!validate_int(input)) continue;         //continue if its not an int

        inputOpt = int_to_options(input);           //convert to the enum

        if (!inputOpt.has_value()) {                //if its not part of the enum
            cout << "Invalid option. Please choose 1 or 2.\n";
            continue;
        }

    } while (!inputOpt.has_value());

    return *inputOpt;
}

Edit edit_or_buy(){
    int num;
    std::optional<Edit> userOpt;

   do{
        cout << "1. Buy a new stock\n2. Edit an existing stock\n";
        if (!validate_int(num)) continue;

        userOpt = int_to_edit(num);                //convert to the enum

        if (!userOpt.has_value())
            cout <<"Invalid input! Enter 1 or 2: ";
            continue;
    } while (!userOpt.has_value());
    
    return *userOpt;
}


int choose_stock(int &max_stock_num){
    int choice;

    while (true){
        cout <<"Which stock do you want to edit? ";
        if (!validate_int(choice)) continue;

        if (choice < 1 || choice > max_stock_num){
            cout <<"Invalid index! "<<endl;
            continue;
        }
        break;   
    }

    return choice;
}

int shares_num(int &max_shares_num){
    int num;

    while (true){
        cout <<"How many shares? ";
        if (!validate_int(num)) continue;

        if (num < 1 || num > max_shares_num){
            cout <<"Invalid index! "<<endl;
            continue;
        }
        break;   
    }

    return num;   
}




