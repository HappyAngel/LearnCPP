#pragma once
#include "event_handler.hpp"
#include "reactor.hpp"

namespace happyangel {
class AcceptorEventHandler : public Event_Handler {
 public:
  AcceptorEventHandler(Reactor *reactor);
  void handle_event(SOCKET s, Event_Type event_type);
  SOCKET get_handle() const;

 private:
  Reactor *reactor_;
};
}  // namespace happyangel