#include <string>
#include "format.h"

using std::string;

string Format::ElapsedTime(long seconds) { 
    long minutes = seconds / 60;

    string f_hours = std::to_string(minutes / 60);
    string f_sec = std::to_string(seconds % 60);
    string f_min = std::to_string(minutes % 60);

    f_hours.insert(f_hours.begin(), 2 - f_hours.length(), '0');
    f_sec.insert(f_sec.begin(), 2 - f_sec.length(), '0');
    f_min.insert(f_min.begin(), 2 - f_min.length(), '0');

    return f_hours + ":" + f_min + ":" + f_sec;
}