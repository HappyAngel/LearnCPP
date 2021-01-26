#pragma once

namespace happyangel {

typedef unsigned int Event_Type;
typedef int SOCKET;


  // Types of indication events.
  #define READ_EVENT  0    // ACCEPT_EVENT aliases READ_EVENT
  #define ACCEPT_EVENT 1  // due to <select> semantics.
  #define WRITE_EVENT  2
  #define TIMEOUT_EVENT 3
  #define SIGNAL_EVENT  4
  #define CLOSE_EVENT  5

class Event_Handler {
public:
		virtual void handle_event(SOCKET s, Event_Type et) = 0;
		virtual SOCKET get_handle() const = 0;
		virtual ~Event_Handler();
};

}  // namespace happyangel