/* This is clock class. Here we got our own clock, which one is running while app is running.
 * Here we can also get information about system time and we can set the time to time which we want.
 * The clock is running in separate thread.
 */

#ifndef CLOCK_H
#define CLOCK_H

#include <string>
#include <mutex>

class Clock
{
public:
    Clock();

    std::string getTime() const;
    bool setTime(std::string time);
    bool isItTimeToCheckTime();
    bool isItTooLateForCheck();
    bool isItTimeToCheckClients();
    bool isTimeToCheckClientsPassed();
    void setSystemTime();
    bool getSecMinHour(int &s, int &m, int &h, std::string &time);

private:
    void run();
    void updateGui();

    std::mutex m_mutex;
    int m_hour,
        m_min,
        m_sec,
        m_checkTime,    //we call all clients to send server their time every x seconds.
        m_checkClient;  //we call all clients to report if they are online to know who is still in our network.

    bool m_timeToCheck;
    bool m_timeToCheckPassed;
    bool m_timeToCheckClients;
    bool m_timeToCheckClientsPassed;
};

#endif // CLOCK_H
