#pragma once
#include <map>
#include <cstddef>

#include "event_handler.hpp"

namespace happyangel {
class Demux_Table {
 public:
  // find accept event
  bool GetAcceptHandlers(Event_Handler*& event_handlers);

  struct Tuple {
    Event_Handler* event_handler;
    Event_Type event_Type;
    Tuple(Event_Handler* eh, Event_Type et) {
      event_handler = eh;
      event_Type = et;
    }
    Tuple(const Tuple& tuple) {
      event_handler = tuple.event_handler;
      event_Type = tuple.event_Type;
    }
    Tuple() {
      event_handler = NULL;
    }
    Tuple& operator=(const Tuple& tuple) {
      event_handler = tuple.event_handler;
      event_Type = tuple.event_Type;
      return *this;
    }
  };
  Tuple* GetTuple(SOCKET h) {
    TupleMap::iterator it = m_TupeTable.find(h);
    if (it != m_TupeTable.end()) {
      return &(it->second);
    }
    return NULL;
  }
  void AddTuple(SOCKET h, Tuple tuple) {
    TupleMap::iterator it = m_TupeTable.find(h);
    if (it != m_TupeTable.end()) {
      it->second.event_Type = it->second.event_Type | tuple.event_Type;
    } else {
      m_TupeTable[h] = tuple;
    }
  }
  bool RemoveTupe(SOCKET h, Event_Type et) {
    TupleMap::iterator it = m_TupeTable.find(h);
    if (it != m_TupeTable.end()) {
      if (it->second.event_Type == et) {
        m_TupeTable.erase(it);
        return true;
      } else if ((it->second.event_Type & et) == et) {
        (it->second.event_Type) &= ~et;
      }
      return true;
    } else {
      return false;
    }
  }
  int GetMaxfd() {
    TupleMap::reverse_iterator iter = m_TupeTable.rbegin();
    return static_cast<int>(iter->first);
  }
  int GetMinfd() {
    TupleMap::iterator iter = m_TupeTable.begin();
    return static_cast<int>(iter->first);
  }
  int GetSize() { return m_TupeTable.size(); }

 private:
  typedef std::map<SOCKET, Tuple> TupleMap;
  TupleMap m_TupeTable;
};
}  // namespace happyangel