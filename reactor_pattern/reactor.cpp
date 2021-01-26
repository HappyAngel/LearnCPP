#include "reactor.hpp"

#include "event_handler.hpp"
#include "reactor_interface.hpp"
#include "epoll_reactor_implementation.hpp"

namespace happyangel {
void Reactor::register_handler(Event_Handler* eh, Event_Type et) {
  reactor_interface_->register_handler(eh, et);
}
void Reactor::remove_handler(Event_Handler* eh, Event_Type et) {
  reactor_interface_->remove_handler(eh, et);
}
void Reactor::handle_events() {
  reactor_interface_->handle_events();
}

static Reactor* pointerInstance;

Reactor* Reactor::instance() {
  if (!pointerInstance) {
    pointerInstance = new Reactor;
    pointerInstance->setReactorInterface(new EPOLL_Reactor_Implementation);
  }
  return pointerInstance;
}
void Reactor::setReactorInterface(Reactor_Interface* reactorInterface) {
  reactor_interface_ = reactorInterface;
}

bool Reactor::initialize() {
  return reactor_interface_->initialize();
}

}  // namespace happyangel