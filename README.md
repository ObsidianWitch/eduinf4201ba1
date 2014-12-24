# TP Sockets #
L'objectif de ce TP est de se familiariser avec les sockets et comment faire communiquer plusieurs applications entre-elles sur un réseau. Deux types de socket seront utilisés, les sockets non connectées (*SOCK_DGRAM*) et les sockets connectées (*SOCK_STREAM*). De plus, le protocole HTTP sera abordé à travers la réalisation d'un client émettant une requête et recevant la réponse correspondante, et la création d'un serveur web répondant à des requêtes.

## Build instructions ##
    mkdir build
    cd build
    cmake ..
    make