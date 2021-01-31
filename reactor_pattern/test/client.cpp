#include <sys/types.h>
#include <sys/socket.h>
#include <cstdlib>
#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <cstring>
#include <cstdio>
#include <errno.h>    //for error handling
#include "../utils.hpp"

# define MESSAGE_SIZE 10
using namespace happyangel;

void send_data(int sockfd) {
    char *query = (char*)malloc(MESSAGE_SIZE + 1);
    for (int i = 0; i < MESSAGE_SIZE; i++) {
        query[i] = 'h';
    }
    query[MESSAGE_SIZE] = '\0';

    const char *cp;
    cp = query;
    size_t remaining = strlen(query);
    while (remaining) {
        int n_written = send(sockfd, cp, remaining, 0);
        fprintf(stdout, "send into buffer %ld \n", n_written);
        if (n_written <= 0) {
            error(EXIT_FAILURE, errno, (char*)"send failed");
            return;
        }
        remaining -= n_written;
        cp += n_written;
    }

    return;
}


int main(int argc, char **argv) {
    int sockfd;
    struct sockaddr_in servaddr;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(9001);
    inet_pton(AF_INET, "127.0.0.1", &servaddr.sin_addr);
    int connect_rt = connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr));
    if (connect_rt < 0) {
        error(EXIT_FAILURE, errno, (char*)"connect failed ");
    }
    
    int i = 5;

    while(i>0) {
        send_data(sockfd);
        i--;
    }

    sleep(3);
    shutdown(sockfd, SHUT_RDWR);
    sleep(2);
    std::cout<<"exit client"<<std::endl;
    exit(0);
}

