#include "client.h"
#include "client_config.h"

#include <iostream>
#include <thread>

int main(){

    std::string buffer;
    sockaddr_in server_address;

    User::UserInfo user;


    Server::ServerSetup(server_address);
    user.socket = socket(AF_INET, SOCK_STREAM, 0);

    User::RegUser(user, server_address);

    std::thread SendMessage(Client::SendMessage, std::ref(user), std::ref(buffer));
    std::thread ReceiveMessage(Client::ReceiveMessage, std::ref(user), std::ref(buffer));

    SendMessage.join();
    ReceiveMessage.join();

    close(user.socket);
    std::cout << "\nend of the session" << std::endl;

    return 0;
}
