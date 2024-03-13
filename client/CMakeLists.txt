set(CLIENT_NAME ${PROJECT_NAME}-client)

set(SOURCE  main.cpp
        client.cpp
        client_config.cpp)

set(HEADERS client.h
        client_config.h)

add_executable(${CLIENT_NAME} ${SOURCE} ${HEADERS})
