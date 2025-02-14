/** \file dab_event_manager.h
*/
#ifndef _dab_event_manager_h_
#define _dab_event_manager_h_

#include <iostream>
#include <typeinfo>
#include <typeindex>
#include <map>
#include <vector>
#include <memory>

namespace dab
{

namespace event
{
    
class Event;
class EventListener;
class EventScheduler;
    
typedef void (*EventHandleFunction)(Event*);

class EventManager
{
public:
	EventManager();
	~EventManager();
	
	EventScheduler* scheduler();
    
	void update();
    
	void addEvent(std::shared_ptr<Event> pEvent);
	void removeEvents();
    
	/**
     \brief register event listener
     \param pEventListener event listener
     */
	template <typename EventType> void addEventListener(std::shared_ptr<EventListener> pEventListener);
    
	/**
     \brief register event listener
     \param pEventListener event listener
     */
	template <typename EventType> void removeEventListener(std::shared_ptr<EventListener> pEventListener);
    
	/**
     \brief inform event listeners about a newly created event
     \param pEvent event
     */
	void notifyEventListeners( std::shared_ptr<Event> pEvent);
    
	/**
     \brief obtain textual event manager information
     \param pPropagate print information for all events in the event scheduler
     \return base::String containing event manager information
     */
	operator std::string() const;
    
	/**
     \brief print event pool information
     \param pOstream output stream
     \param pEventManager event manager
     */
	friend std::ostream& operator<< (std::ostream & pOstream, const EventManager& pEventManager)
	{
		pOstream << pEventManager;
		
		return pOstream;
	}
    
    // debug
    void beep( std::shared_ptr<Event> pEvent );
    // debug done
    
protected:
	EventScheduler* mScheduler;
    
	
	/**
     \brief map from event type to event handler functions
     */
    std::map<const std::type_index, std::vector< std::weak_ptr<EventListener> > > mEventListeners;
    
    
    std::map<Event*, std::vector< std::weak_ptr<EventListener> > > mEventListeners2;
    
    // debug
    std::vector< Event* > mEventTypes;
    // debug end
};
    
template <typename EventType>
void
EventManager::addEventListener(std::shared_ptr<EventListener> pEventListener)
{
    // check if we have already stored this event type
    EventType event;
    std::type_index eventType = std::type_index(typeid(event));
    bool eventTypeStored = false;
    std::cout << "addEventListener typeIndex " << eventType.name() << "\n";
    
    for( auto eventIter = mEventListeners2.begin(); eventIter != mEventListeners2.end(); ++eventIter)
    {
        if( std::type_index(typeid(eventIter->first)) == eventType )
        {
            std::cout << "this event type is already stored\n";
            
            eventIter->second.push_back(pEventListener);
            
            eventTypeStored = true;
            break;
        }
        else
        {
            std::cout << "this event type is not yet stored\n";
        }
    }

    if( eventTypeStored == false )
    {
        EventType* eventPtr = new EventType();
        mEventListeners2[ eventPtr ] = std::vector< std::weak_ptr<EventListener> >();
        mEventListeners2[eventPtr].push_back(pEventListener);
    }
    
    /*
    if( mEventListeners.find(eventType) == mEventListeners.end() )
    {
        mEventListeners[eventType] = std::vector< std::weak_ptr<EventListener> >();
    }
    
    std::vector< std::weak_ptr<EventListener> >& eventListeners = mEventListeners[eventType];
    
    // check if event listener is already registered
    EventListener* listener = pEventListener.get();
    int listenerCount = eventListeners.size();
    for(int i=0; i<listenerCount; ++i)
    {
        if(eventListeners[i].lock().get() == listener) return;
    }
    
    eventListeners.push_back(pEventListener);
    
    // debug
    EventType* event2 = new EventType();
    mEventTypes.push_back( event2 );
    std::cout << "storing event type " << std::type_index( typeid(event2) ).name() << "\n";
    // debug done
     */
}
    
template <typename EventType>
void
EventManager::removeEventListener(std::shared_ptr<EventListener> pEventListener)
{
    EventType event;
    std::type_index eventType = event.eventType();
    
    
}
    

};
    
};

#endif
