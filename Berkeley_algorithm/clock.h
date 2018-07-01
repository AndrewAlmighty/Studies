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
    void setSystemTime();

private:
    void run();
    void updateGui();

    std::mutex m_mutex;
    int m_hour,
        m_min,
        m_sec;
};

#endif // CLOCK_H
