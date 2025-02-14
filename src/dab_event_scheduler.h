/** \file dab_event_scheduler.h
*/

#ifndef _dab_event_scheduler_h_
#define _dab_event_scheduler_h_

#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <mutex>

namespace dab
{

namespace event
{
    
class Event;
class EventManager;

class EventScheduler
{
public:
	EventScheduler(EventManager* pEventManager);
	~EventScheduler();
    
	/**
     \brief return number of events currently scheduled for execution
     \ return number of events currently scheduled for execution
     */
	unsigned int eventCount() const;
	
	/**
     \brief add event to scheduler
     \param pEvent event
     
     adds event to temporary event vector
     */
	void scheduleEvent( std::shared_ptr<Event> pEvent);
	
	/**
     \brief remove all events from scheduler
     */
	void removeEvents();
	
	/**
     \brief manually update event scheduler
     */
	void update();
	
	operator std::string() const;
    
    friend std::ostream& operator << ( std::ostream& pOstream, const EventScheduler& pScheduler)
    {
        pOstream << std::string( pScheduler );
        return pOstream;
    };
    
protected:
    EventScheduler();
    
    /**
     \brief check temporary event vector
     
     removes events from the temporary event vector and:
     a) executes them right away
     b) transfers them into the event list for later execution
     */
	void checkEvents();
    
	/**
     \brief store event in event list
     \param pEvent event to store
     
     inserts event in a temporally sorted manner
     */
	void storeEvent( std::shared_ptr<Event> pEvent);
    
	/**
     \brief processes event list and executes events if necessary
     */
	void processEvents();
    
    /**
    \brief time sorted list of events
    */
    std::list< std::shared_ptr<Event> > mEvents;
        
    /**
    \brief unSorted vector of events

    in order to avoid event being inserted by threads
    while the scheduler is processing threads this vector
    is a temporary storage for events till the scheduler
    has time to insert them into the regular vector
    */
    std::vector< std::shared_ptr<Event> > mTmpEvents;
        
    EventManager* mEventManager;
	std::mutex mLock;
};
    
};
    
};

#endif
