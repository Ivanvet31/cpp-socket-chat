#include "server.h"
#include "server_config.h"

#include <iostream>
#include <termios.h>
#include <thread>
#include <unistd.h>

int main()
{
    Server::Server server;
    std::mutex lock_buf;
    std::string buffer;
    termios settings;

    User::UserInfo *user = new User::UserInfo[server.max_clients];

    tcgetattr(0, &settings);
    settings.c_lflag &= ~ECHO;
    settings.c_lflag &= ~ICANON;
    tcsetattr(0, TCSANOW, &settings);

/* server.log_write.open(server.msg_log, std::ios::app); */

    server.socket = socket(AF_INET, SOCK_STREAM, 0);
    Server::ServerSetup(server.address);
    bind(server.socket, (sockaddr*)&server.address, sizeof(server.address));
    listen(server.socket, server.max_clients);
    std::cout << "ready for connection\n";

    std::thread MsgControl(Client::MsgControl, std::ref(server), std::ref(user),
                           std::ref(lock_buf));

    std::thread ConnectClients(Client::ConnectClients, std::ref(server), std::ref(user));

    std::thread MsgReceive(Client::MsgReceive, std::ref(server),
                           std::ref(user), std::ref(lock_buf));

/*    while(getchar() != '#');

    lock_buf.lock();

    buffer.clear();
    buffer = "#";
    Client::SendAll(buffer, server.max_clients, user);

    lock_buf.unlock();

    server.clients_count = -1;*/

    ConnectClients.join();
    MsgControl.join();
    MsgReceive.join();

    close(server.socket);

    //server.log_write.close();

    delete[] user;

    return 0;
}
