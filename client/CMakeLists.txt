set(CLIENT_NAME ${PROJECT_NAME}-client)

set(SOURCE  main.cpp
        client.cpp
        client_config.cpp)

set(HEADERS client.h
        client_config.h)

set(UTILS_PATH ${PROJECT_SOURCE_DIR}/utils)
include_directories(${UTILS_PATH})
aux_source_directory(${UTILS_PATH} UTILS)

add_executable(${CLIENT_NAME} ${SOURCE} ${HEADERS} ${UTILS})
