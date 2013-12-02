
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <fcntl.h>

// boost includes 
#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/bind.hpp>
#include <iostream>
#include <string>

#include "input_handler.h"
#include "ui_callback.h"
#include "log.h"

InputHandler::InputHandler()
{

}

InputHandler::~InputHandler()
{

}

void InputHandler::init(UICallback *iCallback)
{
    m_ui = iCallback; 
    LOG("Input handler is running...");  
    boost::thread thread(boost::bind(&InputHandler::handleInput, this));
    //thread.join();
}

bool InputHandler::socketRecv(int iSocketFD) 
{
    // reads client message
    char buf[100];
    bzero(buf, 100);
    std::string input; 
    int recvLen;
    if ( (recvLen = recv(iSocketFD, buf, 100, 0)) == -1) {
        perror("Recv");
    } else if (recvLen > 0) {
        input = std::string(buf); 
        LOG_DEBUG("New data received: len=" << recvLen);
        m_ui->handleInput(buf);
    } else { // hup
        close(iSocketFD);
        return false;
    }

    return true;
}

void InputHandler::handleInput()
{
   
    int BACKLOG = 10;
    int sockfd, new_fd;  // listen on sock_fd, new connection on new_fd
    struct sockaddr_in my_addr;    // my address information
    struct sockaddr_in their_addr; // connector's address information
    int yes=1;

    int maxFd = 0;
    fd_set sockets, tempReadFd;
    FD_ZERO(&sockets);
    FD_ZERO(&tempReadFd);


    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }

    if (setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int)) == -1) {
        perror("setsockopt");
        exit(1);         
    }

    my_addr.sin_family = AF_INET;         // host byte order
    my_addr.sin_port = htons(9999);     // short, network byte order
    my_addr.sin_addr.s_addr = htonl(INADDR_ANY); // automatically fill with my IP
    bzero(&my_addr.sin_zero, sizeof(my_addr.sin_zero));

    if (bind(sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) == -1) {
        perror("bind");
        exit(1);
    }

    if (listen(sockfd, BACKLOG) == -1) {
        perror("listen");
        exit(1);
    }

    unsigned int ssz = sizeof(struct sockaddr_in);
    socklen_t *len = (socklen_t*)&ssz;
    maxFd = sockfd;
    FD_SET(sockfd, &sockets);

    LOG("Input Server ready (fd=" << sockfd << ")...");

    int selectRet = 0;
    new_fd = -1; 

    while (true) {  // main accept() loop

        tempReadFd = sockets;
        struct timeval timeout;
        timeout.tv_sec = 3;
        timeout.tv_usec = 0;

        selectRet = TEMP_FAILURE_RETRY(select(maxFd + 1, &tempReadFd, NULL, NULL, &timeout));
        //LOG_DEBUG("Select finished with: " << strerror(selectRet) << " errno: " << selectRet);
        if (selectRet == -1) { // polling sockets 
            perror("Select error");
            exit(-1);
        }

        for (int currentFd = 0; currentFd <= maxFd; currentFd++) {  // checking each socket
            if (FD_ISSET(currentFd, &tempReadFd)) { // socket state changed
                if (currentFd == sockfd) { // server socket changed - we got a new connection

                    if ((new_fd = accept(sockfd, (struct sockaddr *)&their_addr, len)) == -1) {
                        perror("accept");
                        continue;
                    }
                    FD_SET(new_fd, &sockets);
                    if (new_fd > maxFd)
                        maxFd = new_fd;
                    LOG_DEBUG("server: got connection from: " << inet_ntoa(their_addr.sin_addr));

                } else if (currentFd == new_fd) { // client sent message
                    if (!socketRecv(currentFd)) {
                        FD_CLR(currentFd, &sockets);
                        maxFd--;
                    }
                }
            }
        } // for end

    } // while end
}

