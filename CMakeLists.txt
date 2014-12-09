cmake_minimum_required(VERSION 2.6)
project(TP1_2)

SET(CMAKE_C_COMPILER "gcc")
SET(CMAKE_C_FLAGS "-Wall -Wextra -std=c99 -D_POSIX_SOURCE")

include_directories(src)

# Exercice 1
add_executable(exo1_client.out
    src/exo1/client.c
    src/exo1/msg_tools.c
    src/exo1/msg_tools.h
    src/socket_tools.c
    src/socket_tools.h
)

add_executable(exo1_server.out
    src/exo1/server.c
    src/exo1/msg_tools.c
    src/exo1/msg_tools.h
    src/socket_tools.c
    src/socket_tools.h
)

# Exercice 2
add_executable(exo2_client.out
    src/exo2/client.c
    src/socket_tools.c
    src/socket_tools.h
)

add_executable(exo2_server.out
    src/exo2/server.c
    src/socket_tools.c
    src/socket_tools.h
    src/file_tools.c
    src/file_tools.h
    src/http_tools.c
    src/http_tools.h
)

# Exercice 2 alternative
add_executable(exo2_client_alt.out
    src/exo2_alt/client.c
    src/socket_tools.c
    src/socket_tools.h
)

# Exercice 3
add_executable(exo3_client.out
    src/exo3/client.c
    src/socket_tools.c
    src/socket_tools.h
    src/file_tools.c
    src/file_tools.h
    src/http_tools.c
    src/http_tools.h
)

# Exercice 4
add_executable(exo4_server.out
    src/exo4/server.c
    src/socket_tools.c
    src/socket_tools.h
    src/http_tools.c
    src/http_tools.h
    src/file_tools.c
    src/file_tools.h
)

# Exercice 5
add_executable(exo5_client.out
    src/exo5/client.c
    src/socket_tools.c
    src/socket_tools.h
    src/file_tools.c
    src/file_tools.h
    src/http_tools.c
    src/http_tools.h
)
