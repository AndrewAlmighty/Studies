#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include "message.h"
#include "networkmethods.h"

#include <string>
#include <mutex>

class GuiController;

class Controller
{
public:
    Controller(GuiController* ptr = nullptr);
    ~Controller();
    bool connectTo(const std::string &ip, const unsigned &port);
    bool setNetworking(const std::string &ip, const unsigned &port);
    bool isReady() const;
    void enterToCriticalSection();
    void leaveCriticalSection();
    void reset();

private:
    void loop();
    void handleMsg(Message &msg);

    GuiController* m_GuiPtr;
    bool m_IsReady = false;
    int m_Socket = 0;

    std::string m_HostIp = "";
    int m_port = 0;

    bool m_LoopOn = true;
    std::mutex m_mutex;
};

#endif // CONTROLLER_HPP
