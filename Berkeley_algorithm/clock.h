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
    bool setIsAppRunning(bool turnOn);
    bool setTime(std::string time);
    bool isItTimeToCheckTime();
    bool isItTooLateForCheck();
    bool isItTimeToCheckClients();
    void restartClientsStopwatch();
    bool isServerIsOffline() const;
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
        m_checkClient,  //we call all clients to report if they are online to know who is still in our network.
        m_clientsStopwatch;

    bool m_appIsRunning;    //with this I turn on all things thich I need when app is working
    bool m_timeToCheck;     //time to check what time is it
    bool m_timeToCheckPassed;   //time to send client's time passed.
    bool m_timeToCheckClients;  //time to check if clients are still online
    bool m_serverIsSilentTooLong;    //if server is silent for too long time, it means that he is shut down.
};

#endif // CLOCK_H
