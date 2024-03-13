#pragma once

#include "server_config.h"

#include <string>
#include <mutex>
#include <netinet/in.h>

namespace User {
    struct UserInfo {
        sockaddr_in address;
        std::string password;
        std::string login;
        long timeout = 0;
        int socket = -1;
    };

    void RegUser(char *rcv_msg, User::UserInfo &user);

    void DeleteUser(User::UserInfo &user);
}

namespace Client {
    void SendAll(std::string &buffer, const int &max_clients,  User::UserInfo *user);

    void ConnectClients(Server::Server &server, User::UserInfo *user);

    void MsgControl(Server::Server &server, User::UserInfo *user, std::mutex &lock_buf);

    void MsgReceive(Server::Server &server,
                    User::UserInfo *user, std::mutex &lock_buf);

    //void WriteToFile(std::ofstream& log_write, const char* message);

    //void SendData(Server::Server& server, const User::UserInfo& user);
}
