#pragma once

#include <netinet/in.h>

namespace Server {
    void ServerSetup(sockaddr_in &server_address);
}
