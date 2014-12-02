cmake_minimum_required(VERSION 2.6)
project(TP1_2)

SET(CMAKE_CXX_COMPILER "gcc")
SET(CMAKE_CXX_FLAGS "-Wall -Wextra -std=c99")

# Exercice 1
add_executable(exo1_client.out
    exo1/client.c
    exo1/msg_tools.c
    exo1/msg_tools.h
    socket_tools.c
    socket_tools.h
)

add_executable(exo1_server.out
    exo1/server.c
    exo1/msg_tools.c
    exo1/msg_tools.h
    socket_tools.c
    socket_tools.h
)

#TODO Exercice 1 alternative (using getaddrinfo)
#add_executable(exo1_alt_client.out
#    exo1_alt/client.c
#    exo1/tools.c
#    exo1/tools.h
#)

#add_executable(exo1_alt_serveur.out
#    exo1_alt/serveur.c
#    exo1/tools.c
#    exo1/tools.h
#)

# Exercice 2
add_executable(exo2_client.out
    exo2/client.c
)

add_executable(exo2_server.out
    exo2/server.c
    socket_tools.c
    socket_tools.h
)

# Exercice 3
add_executable(exo3_client.out
    exo3/client.c
    socket_tools.c
    socket_tools.h
)

# TODO Exercice 4
