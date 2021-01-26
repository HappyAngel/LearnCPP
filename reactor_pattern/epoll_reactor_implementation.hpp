#pragma once

#include <sys/epoll.h>

#include "reactor_interface.hpp"
#include "demux_table.hpp"

namespace happyangel {
#define MAX_EPOLL_EVENTS 128
#define SERV_PORT 9001

class EPOLL_Reactor_Implementation : public Reactor_Interface {
 public:
  void register_handler(Event_Handler *eh, Event_Type et);
  void remove_handler(Event_Handler *eh, Event_Type et);
  void handle_events();
  bool initialize();

 private:
  int tcp_nonblocking_server_listen(int port);

  Demux_Table demux_table_;
  struct epoll_event *events_;
  int efd_;
  int listen_fd_;
};
}  // namespace happyangel