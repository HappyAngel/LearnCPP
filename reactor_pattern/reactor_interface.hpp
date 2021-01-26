#pragma once

namespace happyangel {
class Event_Handler;
typedef unsigned int Event_Type;

class Reactor_Interface {
 public:
  void virtual register_handler(Event_Handler *eh, Event_Type et) = 0;
  void virtual remove_handler(Event_Handler *eh, Event_Type et) = 0;
  void virtual handle_events() = 0;
  bool virtual initialize() = 0;
};
}  // namespace happyangel