#include "server.h"
#include "server_config.h"

#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include <thread>

void User::RegUser(char *reg_info, User::UserInfo &user)
{
    int i;

    for (i = 0; reg_info[i] != ' ' && i < sizeof(reg_info); ++i);

    reg_info[i] = '\0';

    user.login = (std::string )reg_info;
    user.password = (std::string)(reg_info + i + 1);
}

void User::DeleteUser(User::UserInfo &user) {
    close(user.socket);
    user.password[0] = user.login[0] = '\0';
    user.socket = -1;
}

/*void Client::WriteToFile(std::ofstream& log_write, const char* message){
    log_write << message;
}*/


/*void Client::SendData(Server::Server& server, const User::UserInfo& user)
{
    //::cout << "sending data to the client\n";
    const int buf_len = 4096;
    char buffer[buf_len] = {};
    int file = open(server.msg_log, O_RDONLY);

    size_t len = read(file, buffer, buf_len);
    buffer[len] = '\0';

    if(len > buf_len - 1){
        send(user.socket, buffer + len - 4096, 4096, 0);
    }else{
        send(user.socket, buffer, 4096, 0);
    }

    //std::cout << "data has been sent\n";
    close(file);
}*/

void Client::SendAll(std::string &buffer, const int &max_clients,  User::UserInfo *user) {
    for (int i = 0; i < max_clients; ++i)
    {
        if(user[i].socket != -1 && buffer[0] != '\0') {
            send(user[i].socket, buffer.c_str(), buffer.size() + 1, 0);
        }
    }
}

void Client::ConnectClients(Server::Server &server, User::UserInfo *user)
{
    while (server.clients_count != -1)
    {
        if (server.clients_count < server.max_clients)
        {
            for (int i = 0; i < server.max_clients; ++i)
            {
                if (user[i].socket == -1)
                {
                    socklen_t client_length = sizeof(user[i].socket);
                    user[i].socket = accept(server.socket,(sockaddr *) &user[i].address,
                                            &client_length);

                    ++server.clients_count;

                    //Client::SendData(server,user[i]);
                }
            }
        }
    }

    for (int i = 0; i < server.max_clients; ++i)
    {
        if (user[i].socket != -1) {
            close(user[i].socket);
        }
    }
}

void Client::MsgReceive(Server::Server &server, User::UserInfo *user, std::mutex &lock_buf)
{
    while (server.clients_count != -1)
    {
        for (int i = 0; i < server.max_clients; ++i)
        {

            if (user[i].socket != -1)
            {
                char rcv_msg[1024] = {0};

                if (user[i].timeout > 100000000000)
                {
                    User::DeleteUser(user[i]);

                    lock_buf.lock();
                    server.buffer.append("user " + user[i].login + " has been disconnected\n");
                    --server.clients_count;
                    lock_buf.unlock();

                    continue;
                }

                lock_buf.lock();
                if (recv(user[i].socket, rcv_msg, 1024, MSG_DONTWAIT) == 0)
                {
                    ++user[i].timeout;
                    lock_buf.unlock();

                    continue;
                }
                lock_buf.unlock();

                if (rcv_msg[0] == '@')
                {
                    User::RegUser(rcv_msg + 1, user[i]);
                    continue;
                }

                user[i].timeout = 0;

                lock_buf.lock();
                server.buffer.append((std::string)rcv_msg);
                lock_buf.unlock();
            }
        }
    }
}

void Client::MsgControl(Server::Server &server, User::UserInfo *user, std::mutex &lock_buf)
{
    size_t msg_len = 0;
    std::string msg;
    char ch;

    msg.append("SERVER -> ");
    msg_len = msg.size();
    std::cout << "SERVER -> ";

    std::thread InputManager([&]
    {
        for (;;)
        {
            if ((ch = getchar()) != '\n' && ch != '#') {
                std::cout << ch;
                if (ch == '\b' && msg_len != 0) {
                    std::cout << "\b \b";
                    --msg_len;
                    msg.pop_back();
                    continue;
                }

                msg.push_back(ch);
                ++msg_len;

            } else if (ch == '\n') {
                std::cout << std::endl;
                Client::SendAll(msg, server.max_clients, user);
                msg.clear();

                msg.append("SERVER -> ");
                msg_len = msg.size();
                std::cout << "SERVER -> ";

            } else if (ch == '#') {
                break;
            }
        }

        server.clients_count = -1;
    });

    while(server.clients_count != -1)
    {
        std::thread Send([&]
        {
            if (!server.buffer.empty()) {
                lock_buf.lock();

                for (int i = 0; i < msg_len; ++i) {
                    std::cout << "\b \b";
                }
                std::cout << server.buffer << std::endl;

                Client::SendAll(server.buffer, server.max_clients, user);
                /* Client::WriteToFile(server.log_write, server.buffer.c_str()); */

                server.buffer.clear();
                lock_buf.unlock();

                std::cout << msg;
                fflush(stdout);
            }
        });

        fflush(stdout);
        Send.join();
        /* Client::WriteToFile(server.log_write, msg.c_str()); */
    }

    InputManager.join();
}
