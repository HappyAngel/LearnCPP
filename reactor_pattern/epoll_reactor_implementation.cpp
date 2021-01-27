#include "epoll_reactor_implementation.hpp"

#include <sys/types.h>
#include <sys/socket.h>
#include <cstdlib>
#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <strings.h>
#include <netinet/in.h>
#include "utils.hpp"

namespace happyangel {
void EPOLL_Reactor_Implementation::register_handler(Event_Handler* eh,
                                                    Event_Type et) {
  if (et == ACCEPT_EVENT) {
    demux_table_.AddTuple(listen_fd_, Demux_Table::Tuple(eh, et));
  } else {
    SOCKET s = eh->get_handle();
    demux_table_.AddTuple(s, Demux_Table::Tuple(eh, et));
  }
}

void EPOLL_Reactor_Implementation::remove_handler(Event_Handler* eh,
                                                  Event_Type et) {
  SOCKET s = eh->get_handle();
  demux_table_.RemoveTupe(s, et);
}

bool EPOLL_Reactor_Implementation::initialize() {
  std::cout<<"endtering EPOLL_Reactor_Implementation::initialize()"<<std::endl;
  struct epoll_event event;

  listen_fd_ = tcp_nonblocking_server_listen(SERV_PORT);

  efd_ = epoll_create1(0);
  if (efd_ == -1) {
    error(1, errno, "epoll create failed");
  }

  event.data.fd = listen_fd_;
  event.events = EPOLLIN | EPOLLET;
  if (epoll_ctl(efd_, EPOLL_CTL_ADD, listen_fd_, &event) == -1) {
    error(1, errno, "epoll_ctl add listen fd failed");
  }

  /* Buffer where events are returned */
  events_ = (struct epoll_event*)calloc(MAX_EPOLL_EVENTS, sizeof(event));
  return true;
}

void EPOLL_Reactor_Implementation::handle_events() {
  std::cout<<"start to do epoll wait..."<<std::endl;

  int n = epoll_wait(efd_, events_, MAX_EPOLL_EVENTS, -1);
  
  int socket_fd;
  for (int i = 0; i < n; i++) {
    if ((events_[i].events & EPOLLERR) || (events_[i].events & EPOLLHUP)) {
      std::cerr << "epoll error" << std::endl;
      close(events_[i].data.fd);
      continue;
    } else if (listen_fd_ == events_[i].data.fd) {
      struct sockaddr_storage ss;
      socklen_t slen = sizeof(ss);
      int fd = accept(listen_fd_, (struct sockaddr*)&ss, &slen);
      if (fd < 0) {
        std::cerr << "accept failed" << std::endl;
        continue;
      } else {
        struct epoll_event event;
        fcntl(fd, F_SETFL, O_NONBLOCK);
        event.data.fd = fd;
        event.events = EPOLLIN | EPOLLET;  // edge-triggered
        if (epoll_ctl(efd_, EPOLL_CTL_ADD, fd, &event) == -1) {
          error(1, errno, "epoll_ctl add connection fd failed");
        }
        // call accept event handler
        Demux_Table::Tuple* pTuple = demux_table_.GetTuple(listen_fd_);
				if (pTuple)
				{
					if (pTuple->event_Type == ACCEPT_EVENT)
					{
						pTuple->event_handler->handle_event(fd, ACCEPT_EVENT);
					}
				}
      }
      continue;
    } else {
      socket_fd = events_[i].data.fd;
      if (events_[i].events & EPOLLIN) {
        // read
				Demux_Table::Tuple* pTuple = demux_table_.GetTuple(socket_fd);
				if (pTuple)
				{
					if (pTuple->event_Type == READ_EVENT)
					{
						pTuple->event_handler->handle_event(socket_fd, READ_EVENT);
					}
				}
      }

      if (events_[i].events & EPOLLOUT) {
        // write
				Demux_Table::Tuple* pTuple = demux_table_.GetTuple(socket_fd);
				if (pTuple)
				{
					if (pTuple->event_Type == WRITE_EVENT)
					{
						pTuple->event_handler->handle_event(socket_fd, WRITE_EVENT);
					}
				}
      }

      if (events_[i].events & EPOLLRDHUP) {
        // close
				Demux_Table::Tuple* pTuple = demux_table_.GetTuple(socket_fd);
				if (pTuple)
				{
					if (pTuple->event_Type == CLOSE_EVENT)
					{
						pTuple->event_handler->handle_event(socket_fd, CLOSE_EVENT);
					}
				}
      }
    }
  }
}

int EPOLL_Reactor_Implementation::tcp_nonblocking_server_listen(int port) {
  int listenfd;
  listenfd = socket(AF_INET, SOCK_STREAM, 0);

  // make non-blocking
  fcntl(listenfd, F_SETFL, O_NONBLOCK);

  struct sockaddr_in server_addr;
  bzero(&server_addr, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  server_addr.sin_port = htons(port);

  int on = 1;
  setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

  int rt1 =
      bind(listenfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
  if (rt1 < 0) {
    error(1, errno, "bind failed ");
  }

  int rt2 = listen(listenfd, 1024);
  if (rt2 < 0) {
    error(1, errno, "listen failed ");
  }
  return listenfd;
}

}  // namespace happyangel