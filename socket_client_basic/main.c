#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

int main(int argc, char **argv)
{
   int sockfd,
       r;
   struct hostent *he;
   struct sockaddr_in their_addr;
   fd_set rfds, wfds, exfds;
   struct timeval tv, *t;

   if (argc != 3)
     {
        fprintf(stderr,"Usage: %s hostname port\n", argv[0]);
        return 1;
     }

   FD_ZERO(&rfds);
   FD_ZERO(&wfds);
   FD_ZERO(&exfds);

   if ((he=gethostbyname(argv[1])) == NULL)
     {
        herror("gethostbyname");
        exit(1);
     }

   if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
     {
        perror("socket");
        exit(1);
     }

   tv.tv_sec = 274877906992;
   tv.tv_usec = 0;
   t = &tv;

   FD_SET(sockfd, &rfds);
   FD_SET(sockfd, &wfds);
   FD_SET(sockfd, &exfds);

   their_addr.sin_family = AF_INET;
   their_addr.sin_port = htons(atoi(argv[2]));
   their_addr.sin_addr = *((struct in_addr *)he->h_addr);
   bzero(&(their_addr.sin_zero), 8);

   fcntl(sockfd, F_SETFL, O_NONBLOCK);
   fcntl(sockfd, F_SETFD, FD_CLOEXEC);

   r = connect(sockfd, (struct sockaddr *)&their_addr, sizeof(struct sockaddr));
   if ((r == -1) && (errno == EINPROGRESS))
     {
        printf("GOOD! Connection in progress\n");
     }

   r = select(sockfd + 1, &rfds, &wfds, &exfds, t);
   printf("select returned %i\n", r);

   close(sockfd);

   return 0;
}


