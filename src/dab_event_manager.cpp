/** \file dab_event_manager.cpp
*/

#include "dab_event_manager.h"
#include "dab_event_scheduler.h"
#include "dab_event_listener.h"
#include "dab_event.h"
#include <sstream>
#include <typeinfo>

using namespace dab;
using namespace dab::event;

EventManager::EventManager()
{
    mScheduler = new EventScheduler(this);
}

EventManager::~EventManager()
{
    delete mScheduler;
}

EventScheduler*
EventManager::scheduler()
{
    return mScheduler;
}

void
EventManager::update()
{
    mScheduler->update();
}

void
EventManager::addEvent( std::shared_ptr<Event> pEvent)
{
    mScheduler->scheduleEvent(pEvent);
}

void
EventManager::removeEvents()
{
    mScheduler->removeEvents();
}

void
EventManager::notifyEventListeners( std::shared_ptr<Event> pEvent)
{
    std::type_index eventType = std::type_index(typeid(pEvent.get()));
    
    //std::cout << "notifyEventListeners eventType " << eventType.name() << "\n";
    
    for(auto eventIter = mEventListeners2.begin(); eventIter != mEventListeners2.end(); ++eventIter)
    {
        Event* storedEvent = eventIter->first;
        
        if( storedEvent->isDerivedClass(pEvent.get()) )
        {
            std::vector< std::weak_ptr<EventListener> >& eventListeners = eventIter->second;
            
            //std::cout << "listenerCount " << eventListeners.size() << "\n";
            
            for(auto listenerIter=eventListeners.begin(); listenerIter != eventListeners.end(); ++listenerIter)
            {
                listenerIter->lock()->notify(pEvent);
            }
        }
    }
    
//    auto iter = mEventListeners.find(eventType);
//    
//    if(iter == mEventListeners.end()) return;
//    
//    std::vector< std::weak_ptr< EventListener> >& eventListeners = (*iter).second;
//    
//    int eventListenerCount = eventListeners.size();
//    
//    // check for removed event listeners
//    for(int i=eventListenerCount - 1; i >= 0; --i)
//    {
//        if( eventListeners[i].expired() == true )
//        {
//            eventListeners.erase( eventListeners.begin() + i );
//        }
//    }
//    
//    eventListenerCount = eventListeners.size();
//    
//    // notify event listeners
//    for(int i=0; i<eventListenerCount; ++i)
//    {
//        if(auto eventListenerPtr = eventListeners[i].lock())
//        {
//            (*eventListenerPtr).notify(pEvent);
//        }
//    }
}

EventManager::operator std::string() const
{
    std::stringstream ss;
    
    ss << "EventManager:\n";
    ss << *mScheduler << "\n";

    return ss.str();
}

void
EventManager::beep( std::shared_ptr<Event> pEvent )
{
    std::cout << "beep begin\n";
    
    //std::type_index( typeid(*this) )

    for(int i=0; i<mEventTypes.size(); ++i)
    {
        if( pEvent->isDerivedClass( mEventTypes[i] ) )
        {
            std::cout << "i " << i << " is match\n";
        }
        else
        {
            std::cout << "i " << i << " is not match\n";
        }
    }
    
    std::cout << "beep end\n";
}

