/** \file dab_event_listener.h
*/

#ifndef _dab_event_listener_h_
#define _dab_event_listener_h_

#include <iostream>

namespace dab
{

namespace event
{
    
class Event;

class EventListener
{
public:
    /**
     \brief event notification
     \param pEvent newly created event
     */
	virtual void notify( std::shared_ptr<Event> pEvent) = 0;
};
    
};

}

#endif
