set(SERVER_NAME ${PROJECT_NAME}-server)

set(SOURCE  main.cpp
        server.cpp
        server_config.cpp)

set(HEADERS server.h
        server_config.h)

set(UTILS_PATH ${PROJECT_SOURCE_DIR}/utils)
include_directories(${UTILS_PATH})
aux_source_directory(${UTILS_PATH} UTILS)

add_executable(${SERVER_NAME} ${SOURCE} ${HEADERS} ${UTILS})
