#include "netlib.h"


void usage(char *prog, char *filename)
{
     printf("usage: %s <add data to %s>\n",prog, filename);
     exit(0);
}


void fatal(char *message)
{
     char error_message[100];

     strcpy(error_message, "[!!] warning error");
     strncat(error_message, message,79);
     perror(error_message);
     exit(-1);
}


void *ec_malloc(unsigned int size)
{
     void *ptr;
     ptr = malloc(size);
     if (ptr == NULL) {
          fatal("ec_malloc() error occured");
     }
     return ptr;

}

void dump(const unsigned char *data_buffer, const unsigned int length)
{
     unsigned char byte;
     unsigned int i ,j;
     for (i = 0; i < length; i++) {
          byte = data_buffer[i];
          printf("%02x ", data_buffer[i]);
          if (((i%16) == 15) || (i == length - 1)) {
               for (j=0; j < 15-(i%16); j++) {
                    printf("  ");

               }
               printf("| ");
               for (j = (i - (i % 16)); j <= i; j++) {
                    byte = data_buffer[j];
                    if ((byte > 31) && (byte < 127)) {
                         printf("%c", byte);
                    }else{
                         printf(".");
                    }
               }
               printf("\n");
          }//end if
     }// endfor

}


int
send_string(int sockfd, char *buffer)
{
     int sent_bytes, bytes_to_send;
     bytes_to_send = strlen(buffer);
     while (bytes_to_send > 0) {
          sent_bytes = send(sockfd, buffer, bytes_to_send, 0);
          if (sent_bytes == -1) {
               return (0);
          }
          bytes_to_send -= sent_bytes;
          buffer += sent_bytes;
     }
     return 1;
}


int
recv_line(int sockfd, char *dest_buffer)
{
#define EOL "\r\n"
#define EOL_SIZE 2

     char *ptr;
     int eol_matched = 0;

     ptr = dest_buffer;
     while (recv(sockfd,ptr, 1, 0) == 1) {//1 byte read
          if (*ptr == EOL[eol_matched]) {
               eol_matched++;
                    
               if (eol_matched == EOL_SIZE) {
                    *(ptr+1 - EOL_SIZE) = '\0';//without EOL
                    return strlen(dest_buffer);
               }
          }else{
               eol_matched = 0;
          }
          ptr++;
     }
     return 0;
}


int
getRequest(int sockfd,char *path)
{
     char sendline[4096], recvline[4096];
     int n;
     
     
     snprintf(sendline, sizeof(sendline), "GET %s HTTP/1.0\r\n\r\n",path);

     /*  HTTPリクエスト送信  */
     Writen(sockfd , sendline,strlen(sendline));
     while((n = read_string(sockfd, recvline)) > 0)
          Fputs(recvline,stdout);
     close(sockfd);
     return 0;
}

int read_string(int sockfd, char *buf)
{
     int read_bytes, bytes_to_read;
     bytes_to_read = strlen(buf);
     while (bytes_to_read > 0) {
          read_bytes = read(sockfd, buf, bytes_to_read);
          if (read_bytes == 0) {
               return 0;
          }
          bytes_to_read -= read_bytes;
          buf += read_bytes;
     }
     return 1;
}


void
Fputs(const char *ptr, FILE *stream)
{
	if (fputs(ptr, stream) == EOF)
		fatal("fputs error");
}


char *
Fgets(char *ptr, int n, FILE *stream)
{
     char *rptr;
     if ((rptr = fgets(ptr, n,stream)) == NULL && ferror(stream)) {
          fatal("fgets error");
     }
     return (rptr);
}

ssize_t
writen(int fd, const void *vptr, size_t n)
{
     size_t nleft;
     ssize_t nwritten;
     const char *ptr;
     ptr = vptr;
     nleft = n;
     
     while (nleft > 0) {
          if ((nwritten = write(fd, ptr , nleft)) <= 0) {
               if ((nwritten < 0 && errno == EINTR)) {
                    nwritten = 0;
               }else{
                    return (-1);
               }
          }
          nleft -= nwritten;
          ptr += nwritten;
     }
     return (n);
}

void
Writen(int fd, void *ptr, size_t nbytes)
{
     if (writen(fd, ptr, nbytes) != (ssize_t)nbytes)
          fatal("writen error");
}

static int	read_cnt;
static char	*read_ptr;
static char	read_buf[512];


static ssize_t
my_read(int fd, char *ptr)
{

     read_cnt = 0;
     if (read_cnt <= 0) {
     again:
          if ( (read_cnt = read(fd, read_buf, sizeof(read_buf))) < 0) {
               if (errno == EINTR)
                    goto again;
               return(-1);
               
          } else if (read_cnt == 0)
               return (0);
          
          read_ptr = read_buf;
     }

     read_cnt--;
     *ptr = *read_ptr++;
     return(1);
}



ssize_t
readline(int fd, void *vptr, ssize_t maxlen)
{
     ssize_t	n, rc;
     char	c, *ptr;

     ptr = vptr;
  
     for (n = 1; n < maxlen; n++) {
          if ( (rc = my_read(fd, &c)) == 1) {
               *ptr++ = c;
               if (c == '\n')
                    break;	/* newline is stored, like fgets() */
          } else if (rc == 0) {
               *ptr = 0;
               return(n - 1);	/* EOF, n - 1 bytes were read */
          } else
               return(-1);		/* error, errno set by read() */
     }

     *ptr = 0;	/* null terminate like fgets() */
     return(n);
}


ssize_t
readlinebuf(void **vptrptr)
{
     if (read_cnt)
          *vptrptr = read_ptr;
     return(read_cnt);
}
/* end readline */

ssize_t
Readline(int fd, void *ptr, size_t maxlen)
{
     ssize_t		n;
     if ( (n = readline(fd, ptr, maxlen)) < 0)
          fatal("readline error");
     return(n);
}
