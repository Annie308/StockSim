#include "input.h"
#include "utils.h"
#include "stock_data.h"
#include <iostream>
#include <limits>
#include <ctime>

using namespace std;

bool validate_int(int &input){
    cin >> input;

    if (cin.fail()){
        cout << "Invalid input. Please enter a number\n";
        cin.clear();
        cin.ignore(10000, '\n');
        return false;
    }else{
        cin.ignore(10000, '\n');
        return true;
    }
}

bool validate_bounds(int &input, int &target){
    if (input < target){
        return true;
    }else{
        return false;
    }
}

string get_current_time(){
    time_t now = std::time(nullptr);    //get current time
    tm* local_time = std::localtime(&now);  //convert to local time

    char buffer[20];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M", local_time);
    return string(buffer);
}

