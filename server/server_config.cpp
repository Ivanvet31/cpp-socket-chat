#include "server_config.h"

#include <netinet/in.h>
#include <sys/socket.h>

void Server::ServerSetup(sockaddr_in &server_address) {
    server_address.sin_port = htons(1777);
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_family = AF_INET;
}
