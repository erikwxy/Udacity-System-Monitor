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
    string hour_str, minute_str, sec_str;
    hour = seconds / 3600;
    sec = seconds % 3600;
    minute = sec / 60;
    sec = sec % 60;
    
    hour_str = to_string(hour);
    hour_str.insert(0, 2 - hour_str.length(), '0');

    minute_str = to_string(minute);
    minute_str.insert(0, 2 - minute_str.length(), '0');

    sec_str = to_string(sec);
    sec_str.insert(0, 2 - sec_str.length(), '0');


    time = hour_str + seperation + minute_str + seperation + sec_str;
    
    return time; 
}