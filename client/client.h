#pragma once

#include <string>
#include <mutex>
#include <netinet/in.h>

namespace User {
    struct UserInfo {
        int socket = -1;
        std::string login;
        std::string password;
    };

    void RegUser(User::UserInfo &user, sockaddr_in &server_socket);

    void Disconnect(User::UserInfo &user);
}

namespace Client{
    void SendMessage(User::UserInfo &user, std::string &buffer);

    void ReceiveMessage(User::UserInfo &user, std::string &buffer);
}
