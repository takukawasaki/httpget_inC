#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>





void usage(char *prog, char *filename);
void fatal(char *message);
void *ec_malloc(unsigned int size);
void dump(const unsigned char *data_buffer, const unsigned int length);
int send_string(int ,  char *);
int recv_line(int,  char *);
int getRequest(int ,char *);
ssize_t
writen(int, const void *, size_t );
void
Writen(int, void *, size_t );


static ssize_t
my_read(int, char *);

int read_string(int, char *);

ssize_t
Readline(int , void *, size_t );

ssize_t
readline(int , void *, ssize_t);

char *
Fgets(char *ptr, int n, FILE *stream);

void
Fputs(const char *ptr, FILE *stream);


