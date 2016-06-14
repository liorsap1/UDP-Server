//Name: LIOR SAPIR
//I.D. 304916562
//Login: liorsp
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <signal.h>
#include <ctype.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>
#include <errno.h>
#include "slist.h"

#define MAX_PORT 65535
#define MAX_LEN_REQ 4096

void upperWord(char *word);
void createwordTolist(char *buff, slist_t *list, int port);
int checkIfNumber(char *arg);// check parameters from user
void handleExit(int signum);

slist_t *list;

int main(int argc, char *argv[])
{

  ///////////////////////////////////////////////////////////////////////
  ////////-----------check argument from user--------------//////////////
  ///////////////////////////////////////////////////////////////////////
  if(argc != 2)
  {
    perror("\nusage: ./server.c <port>\n");
    return EXIT_FAILURE;
  }
  int listenPort = 0;
  checkIfNumber(argv[1]);
  if((listenPort = atoi(argv[1])) == 0)///////////////////////////////// letapel
  {
    fprintf(stderr,"\nnumbers: argument port not legal\n"); // in case of wrong input
    fprintf(stderr, "Usage: ./server <port> \n");
    return EXIT_FAILURE;
  }
  if(listenPort < 1 || listenPort > MAX_PORT)
  {
    fprintf(stderr,"\nnumbers: argument port not legal\n"); // in case of wrong input
    fprintf(stderr, "Usage: ./server <port> \n");
    return EXIT_FAILURE;
  }

  ///////////////////////////////////////////////////////////////////////
  ////////--------create a socket--------------------------//////////////
  ///////////////////////////////////////////////////////////////////////

  int srvFd;
  if((srvFd = socket(PF_INET, SOCK_DGRAM, 0)) < 0)// create the socket for 1fd
  {
    perror("\nsocket\n");
    return EXIT_FAILURE;
  }

  ///////////////////////////////////////////////////////////////////////
  ////////--------------bind to port-----------------------//////////////
  ///////////////////////////////////////////////////////////////////////
  struct sockaddr_in srv;
  memset(&srv,0,sizeof(srv));
  srv.sin_family = AF_INET;
  srv.sin_port = htons(listenPort);
  srv.sin_addr.s_addr = htonl(INADDR_ANY);

  if(bind(srvFd, (struct sockaddr*)&srv, sizeof(srv)) < 0 )
  {
    perror("\nbind fail\n");
    return EXIT_FAILURE;
  }


  ///////////////////////////////////////////////////////////////////////
  ////////--------------read from client-------------------//////////////
  ///////////////////////////////////////////////////////////////////////

  struct sockaddr_in cli;
  char buff[MAX_LEN_REQ];
  memset(buff, '\0', sizeof(buff));
  memset(&cli,0,sizeof(cli));
  socklen_t cli_size = sizeof(cli);
  int nbytes;

  fd_set readFds;
  fd_set writeFds;

  signal(SIGINT,handleExit);

  list = (slist_t*)calloc(1, sizeof(slist_t));
  if(list == NULL)
  {
    perror("\nMALLOC\n");
    free(list);
    exit(-1);
  }
  slist_init(list);

  ///////////////////////////////////////////////////////////////////////
  ////////--------------read from client-------------------//////////////
  ///////////////////////////////////////////////////////////////////////

  while(1)
  {
    FD_ZERO(&readFds);
    FD_SET(srvFd, &readFds);
    printf("server is ready to read\n");
    if(select(srvFd+1, &readFds, 0, 0, 0) < 0)
    {
      perror("select");
      slist_destroy(list, SLIST_FREE_DATA);
      free(list);
      exit(EXIT_FAILURE);
    }
    if(FD_ISSET(srvFd, &readFds))
    {
      nbytes = recvfrom(srvFd, buff, sizeof(buff), 0 , (struct sockaddr*)&cli, &cli_size);
      if(nbytes < 0)
      {
        perror("\nrecvfrom\n");
        slist_destroy(list, SLIST_FREE_DATA);
        free(list);
        exit(EXIT_FAILURE);
      }
      if(nbytes == 0)
      {
        memset(buff, '\0', sizeof(buff));
        continue;
      }
      char *fixer = strstr(buff, "\r");
      if(fixer != NULL)
      {
        printf("you got enter\n");
        fixer = '\0';
      }
      upperWord(buff);
      createwordTolist(buff, list,cli.sin_port);
    }

    ///////////////////////////////////////////////////////////////////////
    ////////--------------respond to client------------------//////////////
    ///////////////////////////////////////////////////////////////////////

    FD_SET(srvFd, &writeFds);
    if(select(srvFd+1, 0, &writeFds, 0, 0) < 0)
    {
      perror("select");
      slist_destroy(list, SLIST_FREE_DATA);
      free(list);
      exit(EXIT_FAILURE);
    }
    if(FD_ISSET(srvFd, &writeFds))
    {
      printf("server is ready to write\n");
      int port = 0;
      char *toWrite = slist_pop_first(list,&port);
      if(toWrite != NULL && strlen(toWrite) != 0)
      {
        nbytes = sendto(srvFd, toWrite, strlen(toWrite),0, (struct sockaddr*)&cli, sizeof(cli));
        if(nbytes < 0)
        {
          perror("\nsendto\n");
          slist_destroy(list, SLIST_FREE_DATA);
          free(list);
          exit(EXIT_FAILURE);
        }
        free(toWrite);
      }
    }
    FD_ZERO(&writeFds);
  }
}

///////////////////////////////////////////////////////////////////////
////////--------------private functions------------------//////////////
///////////////////////////////////////////////////////////////////////

void createwordTolist(char *buff, slist_t *list, int port)
{
  if(buff == NULL)
  {
    perror("buff NULL 1");
    exit(EXIT_FAILURE);
  }
  char *temp = (char*)calloc(strlen(buff),sizeof(char));
  if(temp == NULL)
  {
    perror("createwordTolist: problem MALLOC");
    slist_destroy(list, SLIST_FREE_DATA);
    free(list);
    free(temp);
    exit(EXIT_FAILURE);
  }
  memset(temp, '\0', strlen(temp));
  int i;

  for (i = 0; i < strlen(buff); i++)
    temp[i] = buff[i];

  if(slist_append(list, temp, port) != 0)
  {
    perror("createwordTolist: problem MALLOC");
    slist_destroy(list, SLIST_FREE_DATA);
    free(list);
    exit(EXIT_FAILURE);
  }
}


void upperWord(char *word)
{
  int i;
  for (i = 0; i < strlen(word); i++)
  {
    if(word[i] >= 97 && word[i] <=122)
    {
      word[i] = word[i] - 32;
    }
  }
}

int checkIfNumber(char *arg)
{
  int i;
  for (i = 0; i < strlen(arg); i++)
  {
    if(!isdigit(arg[i]))
    {
      perror("createwordTolist: problem MALLOC");
      exit(EXIT_FAILURE);
    }
  }
  return 0;
}

void handleExit(int signum)//for ctrl c
{
	if(signum == SIGINT)
	{
    slist_destroy(list, SLIST_LEAVE_DATA);
		return;
	}
}
