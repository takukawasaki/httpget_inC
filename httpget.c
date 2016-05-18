#include "netlib.h"


int
SocketClose(int);

int
main(int argc, char *argv[])
{
     char buf[512];
     int sockfd;
   

     struct hostent *host;
     struct sockaddr_in addr;

     if (argc != 3) {
          printf("Usage : %s dest path\n", argv[0]);
          return 1;
     }

     if ((host = gethostbyname(argv[1])) == NULL) {
          fatal("search failed");
     }
     if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
          fatal("socket error");
     }

     
     bzero(&addr, sizeof(addr));
     addr.sin_family = AF_INET;
     addr.sin_port = htons(80);
     addr.sin_addr = *((struct in_addr *)host->h_addr);

     
     if(connect(sockfd, (struct sockaddr *)&addr,sizeof(addr)) < 0){
          fatal("connect");
     }


     
     getRequest(sockfd, argv[2]);
     return 0;
}
