#include <chrono>
#include <string>
#include <vector>
#include <iostream>

using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::chrono::utc_clock;
using std::chrono::system_clock;
using std::chrono::duration;
using std::chrono::minutes;
using std::chrono::hours;
using std::chrono::days;




using std::chrono::time_point;
using std::chrono::tzdb;
class participant
{
    public:
    const string name;
    const std::chrono::time_zone *tz;
    participant(const string &n, const std::chrono::time_zone *t): name(n), tz(t){}
};

void display_time_zones(const std::chrono::tzdb &tzdb, const time_point<system_clock> meeting_utc, const vector<participant> &participants)
{
    for (const participant &member : participants)
    {
        const std::chrono::zoned_time local_time{member.tz, meeting_utc};
        cout << member.name << ", your local time for the meeting is: " << local_time << "\n";
    }
}

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

void add_participant(const std::chrono::tzdb& tzdb, vector<participant> &to_add, int &i)
{
    cout << "First name of participant #" << i << ": ";
    string name;
    cin >> name;
    cout << "\nEnter \"Continent/Location\" of participant #" << i << "'s timezone: ";
    ++i;
    string time_zone_init;
    cin >> time_zone_init;
    cout << "\n";
    try
    {
        const std::chrono::time_zone *participant_timezone = tzdb.locate_zone(time_zone_init);
        participant next{name, participant_timezone};
        to_add.push_back(next);
    }
    catch (std::exception e)
    {
        cout << "Something went wrong! Skipping this participant. More details: " << e.what() << endl;
    }
}

int main()
{
    const std::chrono::tzdb& tzdb = my_get_tzdb();
    vector<participant> participants;
    cout << "Meeting organizer - arrange a telephone meeting with people all over the world." << endl;
    cout << "How long until your meeting? (answer in \"days hours minutes\"): " << endl;
    int n_days;
    int n_hours;
    int n_minutes;
    cin >> n_days;
    cin >> n_hours;
    cin >> n_minutes;
    const days add_days(n_days);
    const hours add_hours(n_hours);
    const minutes add_minutes(n_minutes);

     //const std::chrono::zoned_time local_time{member.tz, meeting_utc};
    const auto utc_meeting_time = system_clock::now() + (add_days + add_hours + add_minutes);
    const auto local_meeting_time= tzdb.current_zone()->to_local(utc_meeting_time);
    cout << "Interpreted that the meeting is at:  " << local_meeting_time << endl;
    cout << "(you can press ctrl + c now to force the program to quit, if you are not happy with the time.)" << endl;
    bool more = true;
    int number = 1;
    while (more)
    {
        add_participant(tzdb, participants, number);
        cout << "More? (y/n)" << endl;
        char yn;
        cin >> yn;
        if (yn == 'n')
        {
            more = false;
        }
    }
    display_time_zones(tzdb, utc_meeting_time, participants);
    return 0;
}