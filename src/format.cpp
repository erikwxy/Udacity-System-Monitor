#include <string>

#include "format.h"

using std::string;
using std::to_string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) { 
    int hour{0}, minute{0}, sec{0};
    string time, seperation{":"};
    hour = seconds / 3600;
    minute = (seconds - hour * 3600) / 60;
    sec = seconds - hour * 3600 - minute * 60;
    time = to_string(minute) + seperation + to_string(minute) + seperation + to_string(sec);
    
    return time; 
}