#include "../lib/fonctionsTCP.h"

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

int socketServeur(unsigned short port)
{
  int sock;
  struct sockaddr_in addr;
  socklen_t addrlen = sizeof(struct sockaddr_in);

  // Création de la socket de connexion
  if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
  {
    return -1;
  }

  // Configuration de l'adresse
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  addr.sin_addr.s_addr = INADDR_ANY;

  // TIME_WAIT
  int enable = 1;

  if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
  {
    close(sock);

    return -1;
  }

  // Association de l'adresse à la socket de connexion
  if (bind(sock, (struct sockaddr *)&addr, addrlen) < 0)
  {
    close(sock);

    return -1;
  }

  // Mise en écoute des futures demandes de connexion
  if (listen(sock, 1) < 0)
  {
    close(sock);

    return -1;
  }

  return sock;
}

int socketClient(char *name, unsigned short port)
{
  char service[8];
  int sock;
  struct sockaddr_in addr;
  socklen_t addrlen;
  struct addrinfo hints;
  struct addrinfo *res;

  sprintf(service, "%d", port);

  // Création de la socket
  if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
  {
    return -1;
  }

  // Configuration addrinfo
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = 0;
  hints.ai_protocol = 0;

  // Calcul de l'adresse de la socket serveur
  if (getaddrinfo(name, service, &hints, &res) < 0)
  {
    close(sock);

    return -1;
  }

  addr = *(struct sockaddr_in *)res->ai_addr;
  addrlen = res->ai_addrlen;

  // Demande de connexion à la socket serveur
  if (connect(sock, (struct sockaddr *)&addr, addrlen) < 0)
  {
    close(sock);

    return -1;
  }

  return sock;
}
