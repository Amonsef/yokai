#ifndef TCP_HEADER
#define TCP_HEADER

/**
 * Créer une socket serveur.
 *
 * unsigned short port  Le numéro de port associé à la socket créée.
 * return               -1 si erreur, numéro du descripteur de fichier sinon.
 */
int socketServeur(unsigned short);

/**
 * Créer une socket client.
 *
 * char*          name  L'hôte (nom ou adresse ip) à contacter pour établir une connexion.
 * unsigned short port  Le numéro de port de la socket serveur à contacter.
 * return               -1 si erreur, numéro du descripteur de fichier sinon.
 */
int socketClient(char *, unsigned short);

#endif
