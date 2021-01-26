#pragma once

#include "event_handler.hpp"

namespace happyangel {
class Reactor;

class DummyEventHandler : public Event_Handler {
 public:
  DummyEventHandler(Event_Type et, const SOCKET socket, Reactor *reactor);
  void handle_event(SOCKET s, Event_Type et);
  SOCKET get_handle() const;

 private: 
  SOCKET socket_;
};
}  // namespace happyangel