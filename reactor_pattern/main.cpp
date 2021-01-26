#include <iostream>
#include <cstdlib>

#include "reactor.hpp"
#include "accept_event.hpp"

using namespace happyangel;

int main(int argc, char **argv) {
  if (!Reactor::instance()->initialize()) {
    std::cerr<<"reactor initialize failed"<<std::endl;
    exit(EXIT_FAILURE);
  }

  AcceptorEventHandler *handler =
      new AcceptorEventHandler(Reactor::instance());

  while (true) {
    Reactor::instance()->handle_events();
  }
}