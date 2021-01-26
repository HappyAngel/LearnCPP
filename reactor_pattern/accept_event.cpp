#include "accept_event.hpp"

#include <iostream>

#include "dummy_event_handler.hpp"

namespace happyangel {
AcceptorEventHandler::AcceptorEventHandler(Reactor * reactor) : reactor_(reactor)
{
	reactor_->register_handler(this, ACCEPT_EVENT);
}

void AcceptorEventHandler::handle_event(SOCKET s, Event_Type event_type)
{
	if (event_type == ACCEPT_EVENT) {
        std::cout<<"accept connection  "<< std::endl;
		DummyEventHandler* dummy_handler = new DummyEventHandler(READ_EVENT, s, reactor_);
	}
}

SOCKET AcceptorEventHandler::get_handle() const {
	std::cerr<<"unwanted access to acceptor's get_handler()"<<std::endl;
	return 0;
}
}