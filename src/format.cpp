#include <string>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) 
{ 
    long hrs,mins,secs;
    string hrss, minss,secss;

    // uptime = "00:00:00";

    hrs = seconds/3600;
    hrss = ConverttoString(hrs);

    mins = (seconds/60)%60;
    minss = ConverttoString(mins);

    secs = seconds%60;
    secss = ConverttoString(secs);

    return hrss+":"+minss+":"+secss;

}

string Format::ConverttoString(long time)
{
    if (time < 10)
    {
        return "0"+std::to_string(time);
    }
    else
    {
        return std::to_string(time);
    }
}