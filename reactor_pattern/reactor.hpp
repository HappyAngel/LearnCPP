
#pragma once

#include <sys/time.h>

namespace happyangel {

class Reactor_Interface;
class Event_Handler;
typedef unsigned int Event_Type;

class Reactor {
 public:
  void register_handler(Event_Handler *eh, Event_Type et);
  void remove_handler(Event_Handler *eh, Event_Type et);
  void handle_events();
  static Reactor *instance();
  void setReactorInterface(Reactor_Interface *reactorInterface);
  bool initialize();

 private:
  Reactor_Interface *reactor_interface_;
};

}  // namespace happyangel