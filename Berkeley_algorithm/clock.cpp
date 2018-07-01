#include "clock.h"
#include "guimanager.h"

#include <time.h>
#include <chrono>
#include <thread>
#include <unistd.h>

Clock::Clock()
{
    setSystemTime();
    run();
}

std::string Clock::getTime() const
{
    //Returns time in HH:MM:SS format.
    return std::string(std::to_string(m_hour) + ":" + std::to_string((m_min)) + ":" + std::to_string(m_sec));
}

bool Clock::setTime(std::string time)
{
    //This method sets time in our clock. First we check if string got HH:MM:SS format, then if values are good, then we change clock.

    std::string tmp;
    int pos = 0;
    int h = 0,
        m = 0,
        s = 0;

    for(unsigned i = 0; i < time.length(); i++)
    {
        if(time[i] == ':' || isdigit(time[i]))
            continue;

        else return false;
    }

    for(unsigned i = 0; i < time.length(); i++)
    {
        if(time[i] != ':')
            tmp += time[i];

        else
        {
            if(pos == 0)
                if((h = std::stoi(tmp)) > 23)
                    return false;

            if(pos == 1)
                if((m = std::stoi(tmp)) > 60)
                    return false;

            pos++;
            tmp.clear();
        }
    }

    if((s = std::stoi(tmp)) > 60)
        return false;

    std::lock_guard<std::mutex> lock(m_mutex);
    m_hour = h;
    m_min = m;
    m_sec = s;

    return true;
}

void Clock::setSystemTime()
{
    //Set clock to system time.

    auto systemTime = std::chrono::system_clock::now();

    time_t tt = std::chrono::system_clock::to_time_t(systemTime);
    struct tm * tmp = localtime(&tt);

    std::lock_guard<std::mutex> lock(m_mutex);
    m_hour = tmp -> tm_hour;
    m_min = tmp -> tm_min;
    m_sec = tmp -> tm_sec;
}

void Clock::run()
{
    //This method runs in separate thread for all the time. It's responsible to keep clock running.

    std::thread threadObj([this]{
        while(true)
        {
            sleep(1);
            {
                std::lock_guard<std::mutex> lock(m_mutex);
                m_sec++;

                if(m_sec >= 60)
                {
                    m_min++;
                    m_sec = 0;
                }

                if(m_min >= 60)
                {
                    m_hour++;
                    m_min = 0;
                }

                if(m_hour >= 24)
                    m_hour = 0;

            }
            updateGui();
        }
    });

    threadObj.detach();
}

void Clock::updateGui()
{
    //This method updates clock which one is in GUI.

    if(GuiManager::GetInstance().running() == true)
        GuiManager::GetInstance().setTime(QString(getTime().c_str()));
}
