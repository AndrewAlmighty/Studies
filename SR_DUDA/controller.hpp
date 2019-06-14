#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include "message.h"
#include "networkmethods.h"
#include <unordered_map>
#include <string>
#include <mutex>

class GuiController;

using addr = std::pair<std::string, unsigned>;

struct addrhash {
public:
  template <typename T, typename U>
  std::size_t operator()(const std::pair<T, U> &x) const
  {
    return std::hash<T>()(x.first) ^ std::hash<U>()(x.second);
  }
};

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
    void handleConnectionRequest(std::string ip, unsigned port);

    GuiController* m_GuiPtr = nullptr;
    bool m_IsReady = false;
    bool m_inCritical = false;
    int m_Socket = 0;
    long int m_curTimestamp = 0;
    std::string m_HostIp = "";
    unsigned m_port = 0;
    std::unordered_map<addr, unsigned, addrhash> m_devicesList;
    std::unordered_map<addr, int, addrhash> m_queue;
    bool m_LoopOn = true;
    std::mutex m_mutex;
};

#endif // CONTROLLER_HPP
