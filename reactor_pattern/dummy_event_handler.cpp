#include "dummy_event_handler.hpp"

#include <iostream>
#include <unistd.h>
#include <errno.h>    //for error handling

#include "reactor.hpp"

namespace happyangel {
DummyEventHandler::DummyEventHandler(Event_Type et, SOCKET socket,
                                     Reactor *reactor)
    : socket_(socket) {
  reactor->register_handler(this, et);
}

void DummyEventHandler::handle_event(SOCKET s, Event_Type et) {
  if (et == READ_EVENT) {
    std::cout << "received READ_EVENT" << std::endl;
    int n = 0;
    while (1) {
      char buf[512];
      if ((n = read(s, buf, sizeof(buf))) < 0) {
        if (errno != EAGAIN) {
          std::cerr << "read error" <<std::endl;
          close(s);
        }
        break;
      } else if (n == 0) {
        std::cout<<"no data to read now, exit read event"<<std::endl;
        break;
      } else {
        buf[n] = '\0';
        std::cout<<"received "<<buf<<std::endl;
        if (write(s, buf, n) < 0) {
          std::cerr << "write error" <<std::endl;
        }
      }
    } 
  }

  if (et == WRITE_EVENT) {
    std::cout << "received WRITE_EVENT" << std::endl;   
  } else if (et == CLOSE_EVENT) {
    std::cout << "received CLOSE_EVENT" << std::endl;   
    delete this;
  }
}

SOCKET DummyEventHandler::get_handle() const { return socket_; }

}  // namespace happyangel