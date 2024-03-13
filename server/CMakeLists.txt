set(SERVER_NAME ${PROJECT_NAME}-server)

set(SOURCE  main.cpp
        server.cpp
        server_config.cpp)

set(HEADERS server.h
        server_config.h)

add_executable(${SERVER_NAME} ${SOURCE} ${HEADERS})
