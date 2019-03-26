#include "../lib/fonctionsTCP.h"
#include "../lib/protocolYokai.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>

void closeWithError(int sock, char *err)
{
  printf(err);

  shutdown(sock, SHUT_RDWR);
  close(sock);
}

char *sensToString(TSensTetePiece piece)
{
  switch (piece)
  {
  case NORD:
    return "NORD";

  case SUD:
    return "SUD";

  default:
    return NULL;
  }
}

TPartieReq *createPartieReq(char nomJoueur[T_NOM], TSensTetePiece piece)
{
  TPartieReq *req = calloc(1, sizeof(TPartieReq));

  req->idReq = PARTIE;
  strcpy(req->nomJoueur, nomJoueur);
  req->piece = piece;

  return req;
}

int sendPartieReq(int sock, char nomJoueur[T_NOM], TSensTetePiece piece)
{
  TPartieReq *partieReq = createPartieReq(nomJoueur, piece);

  if (send(sock, partieReq, sizeof(TPartieReq), 0) <= 0)
  {
    free(partieReq);

    return -1;
  }

  free(partieReq);

  return 0;
}

TPartieRep *receivePartieRep(int sock)
{
  TPartieRep *partieRep = calloc(1, sizeof(TPartieRep));

  if (recv(sock, partieRep, sizeof(TPartieRep), 0) <= 0)
  {
    free(partieRep);

    return NULL;
  }

  return partieRep;
}

int main(int argc, char **argv)
{
  if (argc < 4)
  {
    printf("Usage: %s hote port nom\n", argv[0]);

    return EXIT_FAILURE;
  }

  char *host = argv[1];
  int port = atoi(argv[2]);
  char *name = argv[3];

  int sock;

  if ((sock = socketClient(host, port)) < 0)
  {
    printf("Erreur: impossible de créer la socket\n");

    return EXIT_FAILURE;
  }

  TSensTetePiece direction = SUD;

  if (sendPartieReq(sock, name, direction) < 0)
  {
    closeWithError(sock, "Erreur: impossible d'envoyer la requête TPartieReq\n");

    return EXIT_FAILURE;
  }

  TPartieRep *partieRep = receivePartieRep(sock);

  if (partieRep == NULL)
  {
    closeWithError(sock, "Erreur: impossible de recevoir la réponse TPartieRep\n");

    return EXIT_FAILURE;
  }

  if (partieRep->err != ERR_OK)
  {
    closeWithError(sock, "Erreur: la requête TPartieReq envoyée est invalide\n");

    return EXIT_FAILURE;
  }

  if (partieRep->validSensTete == KO)
  {
    direction = direction == SUD ? NORD : SUD;
  }

  char *sens = sensToString(direction);
  char *sensAdvers = sensToString(direction == SUD ? NORD : SUD);

  printf("Lancement de la partie: %s (%s) vs %s (%s)\n", name, sens, partieRep->nomAdvers, sensAdvers);

  free(partieRep);

  return 0;
}
