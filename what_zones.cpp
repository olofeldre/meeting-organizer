#include <iostream>
#include <chrono>
#include <vector>

using std::vector;
using std::cout; using std::endl;

const std::chrono::tzdb& my_get_tzdb()
{
    try
    {
        return std::chrono::get_tzdb();
    }
    catch (std::exception e)
    {
        cout << "Something went wrong initializing the time zone db! " << e.what() << endl;
        throw e;
    }
}

int main()
{
    const std::chrono::tzdb& tzdb = my_get_tzdb();
    const auto &time_zones = tzdb.zones;
    for(const auto &zone : time_zones)
    {
        cout << zone.name() << endl;
        //cout << "Info: " << zone.get_info() << endl;
    }
    return 0;
}