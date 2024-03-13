#pragma once

#include <string>
#include <netinet/in.h>

namespace Server
{
    struct Server
    {
        int socket, clients_count = 0;
        const int max_clients = 5;
        sockaddr_in address;
        std::string buffer;

        /*const char* msg_log = "msg_log.txt";
        std::ofstream log_write;*/
    };

    void ServerSetup(sockaddr_in &server_address);
}
