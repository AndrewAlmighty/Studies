#ifndef BERKELEYMANAGER_H
#define BERKELEYMANAGER_H

#include <string>

class BerkeleyManager
{
public:

    enum Mode{
        NotSpecified,
        Server,
        Client
    };

    BerkeleyManager();

    bool RunAsServer();
    bool RunAsClient(std::string ip);
    void Stop();

    void setMode(Mode m);
    void setID(int id);
    void setTime(std::string time);

    int getID() const;
    std::string getMode() const;
    std::string getIP() const;
    std::string getMAC() const;
    std::string getTime() const;

private:
    Mode m_mode;
    int m_id;
};

#endif // BERKELEYMANAGER_H
