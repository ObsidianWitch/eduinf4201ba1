cmake_minimum_required(VERSION 2.6)
project(TP1_2)

SET(CMAKE_CXX_COMPILER "gcc")
SET(CMAKE_CXX_FLAGS "-Wall -Wextra")

# Exercice 1
add_executable(exo1_client.out
    exo1/client.c
    exo1/tools.c
    exo1/tools.h
)

# TODO tools.c/.h needed here?
add_executable(exo2_serveur.out
    exo1/serveur.c
    exo1/tools.c
    exo1/tools.h
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

# TODO Exercice 2
# TODO Exercice 3
# TODO Exercice 4
