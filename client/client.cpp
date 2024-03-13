#include "client.h"

#include <iostream>
#include <sys/socket.h>
#include <termios.h>
#include <thread>
#include <unistd.h>


void User::RegUser(User::UserInfo &user, sockaddr_in &server_address)
{
    std::string reg_info;

    std::cout << "registration has started\n enter your login and password in format:\n->login password\n->";
    getline(std::cin, reg_info);
    reg_info = "@" + reg_info;

    std::cout << "waiting...";
    int count = 0;

    while(connect(user.socket, (sockaddr*)&server_address, sizeof(server_address)) != 0)
    {
        if (count == 1000000) {
            std::cout << '.';
            count = 0;
        }
        ++count;
    }

    send(user.socket, reg_info.c_str(), reg_info.size() + 1, 0);

    int i = 0;
    while(reg_info[i] != ' '){
        ++i;
    }
    reg_info[i] = '\0';

    user.login = (std::string)(reg_info.c_str() + 1);
    user.password = (std::string)(reg_info.c_str() + i + 1);

    std::cout << '\n';
    fflush(stdout);
    std::cout << (user.login + ", you have been registered successfully\n");

    //recv(user.socket, buffer, sizeof(buffer), 0);
    //std::cout << buffer;
}


void User::Disconnect(User::UserInfo &user)
{
    std::cout << "you have been disconnected\n";
    close(user.socket);
}


void Client::SendMessage(User::UserInfo &user, std::string &buffer)
{
    termios settings;
    std::string msg;
    char ch;

    tcgetattr(0, &settings);
    settings.c_lflag &= ~ECHO;
    settings.c_lflag &= ~ICANON;
    tcsetattr(0, TCSANOW, &settings);

    msg.append(user.login + " -> ");

    std::thread PrintMsg([&]
    {
        while (ch != '#')
        {
            if (!buffer.empty())
            {
                for (int i = 0; i < msg.size(); ++i) {
                    std::cout << "\b \b";
                }

                std::cout << buffer << '\n';
                buffer.clear();
                std::cout << msg;
            }
        }
    });

    while ((ch = getchar()) != '#')
    {
        if (ch != '\n')
        {
            std::cout << ch;
            if (ch == '\b' && msg.size() != 0)
            {
                std::cout << "\b \b";

                msg.pop_back();

                continue;
            }

            msg.push_back(ch);

        } else if (ch == '\n' && !msg.empty()) {

            for(int i = 0; i < msg.size() + 1; ++i){
                std::cout << "\b \b";
            }

            send(user.socket, msg.c_str(), (user.login + " -> " + msg).size() + 1,
                 0);

            msg.clear();
            msg.append(user.login + " -> ");
            std::cout << msg;
        }
    }

    PrintMsg.join();
    std::cout << std::endl;
}


void Client::ReceiveMessage(User::UserInfo &user, std::string &buffer)
{
    while(buffer[0] != '#') {
        char rcv_msg[1024] = {0};

        recv(user.socket, rcv_msg, sizeof(rcv_msg), 0);
        buffer.append((std::string)rcv_msg);
    }

    User::Disconnect(user);
}
