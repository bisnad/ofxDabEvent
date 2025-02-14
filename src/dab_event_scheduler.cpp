/** \file dab_event_scheduler.cpp
*/

#include <sstream>
#include "ofUtils.h"
#include "dab_event_scheduler.h"
#include "dab_event_manager.h"
#include "dab_event.h"

using namespace dab;
using namespace dab::event;

EventScheduler::EventScheduler(EventManager* pEventManager)
: mEventManager(pEventManager)
{}

EventScheduler::~EventScheduler()
{
    removeEvents();
}

unsigned int
EventScheduler::eventCount() const
{
    return mEvents.size();
}

void
EventScheduler::scheduleEvent( std::shared_ptr<Event> pEvent)
{
	mLock.lock();

    mTmpEvents.push_back(pEvent);

	mLock.unlock();
}

void
EventScheduler::removeEvents()
{
    mEvents.clear();
    mTmpEvents.clear();
}

void
EventScheduler::update()
{
    processEvents();
}

EventScheduler::operator std::string() const
{
    std::stringstream ss;
    
    ss << "EventScheduler\n    eventCount: " << mEvents.size() << "\n";
    
    unsigned int eventCount = mEvents.size();
    std::shared_ptr<Event> event;
	
    int eventNr = 0;
	for(auto eventIter = mEvents.begin(); eventIter != mEvents.end(); eventIter++, eventNr++)
	{
		event = *eventIter;
        ss << "    event: " << eventNr << " time: " << event->time() << "\n";
	}
    
    return ss.str();
    
}

void
EventScheduler::checkEvents()
{
	//std::cout << "EventScheduler::checkEvents() begin\n";
    
    //unsigned int tmpEventCount = mTmpEvents.size();

	//std::cout << "tmpEventCount " << tmpEventCount << "\n";

	if(mLock.try_lock() == false) return;

    std::shared_ptr<Event> event;
	//for(unsigned int i=0; i<tmpEventCount; ++i)
	for (auto event : mTmpEvents)
	{
		//event = mTmpEvents[i];
		
        float eventTime = event->time();
        float currentTime = ofGetElapsedTimef();
        
		//std::cout << "check event " << i << " addr " << event << " event time " << eventTime << " current time " << currentTime << " finished " << event->finished() << "\n";
		
		if(eventTime < currentTime)
		{
			//std::cout << "event execute begin\n";
            
			mEventManager->notifyEventListeners( event );
            
            //mEventManager->beep(event);
			
			//std::cout << "i " << i << " event " << event << "\n";
			
			event->execute();
			
			//std::cout << "event execute done\n";
		}
		
		// check if we need to store event for later execution
		if(event->finished() == false)
		{
			//std::cout << "store event " << i << "\n";
            
			storeEvent(event);
			
			//std::cout << "store\n";
		}
		
		//std::cout << i << " " << *event << "\n";
	}
    
    mTmpEvents.clear();

	mLock.unlock();
    
	//std::cout << "EventScheduler::checkEvents() end\n";
}

void
EventScheduler::storeEvent( std::shared_ptr<Event> pEvent)
{
//    std::cout << "EventScheduler::storeEvent(Event* pEvent) begin\n";
//    
//    std::cout << "events scheduler before:\n";
//    std::cout << (*this) << "\n";
    
	// insert event into correct location at events list
	float eventTime = pEvent->time();
	int eventCount = mEvents.size();
	
//	std::cout << "event time " << pEvent->time() << " finished " << pEvent->finished() << " stored -> stored event count " << mEvents.size() << "\n";
    
	if(eventCount == 0) // trivial case, we insert the very first event
	{
		mEvents.push_back(pEvent);
	}
    else if( mEvents.front()->time() > eventTime ) // new event happens sooner than first stored event
    {
        mEvents.push_front(pEvent);
    }
    else // we have to find the right position to insert event (temporally sorted insertion)
    {
        for (auto eventIter = --(mEvents.end()); eventIter != --mEvents.begin(); --eventIter)
        {
            if((*eventIter)->time() <= eventTime)
            {
                auto tmpIter = eventIter;
                mEvents.insert(++tmpIter, pEvent);
                break;
            }
        }
    }
    
//    std::cout << "events scheduler after:\n";
//    std::cout << (*this) << "\n";
//    
//    std::cout << "EventScheduler::storeEvent(Event* pEvent) end\n";
}

void
EventScheduler::processEvents()
{
//    std::cout << "EventScheduler::processEvents() begin\n";
//    
//    std::cout << "events scheduler before:\n";
//    std::cout << (*this) << "\n";
    
    checkEvents();
    
    float currentTime = ofGetElapsedTimef();
    std::shared_ptr<Event> event;
	unsigned int eventCount = mEvents.size();
	int eventIndex = 0;
    
	// execute events
    auto eventIter = mEvents.begin();
    for(; eventIter != mEvents.end(); ++eventIter)
    {
        event = (*eventIter);
        if(event->time() > currentTime) break;
        
        mEventManager->notifyEventListeners( event );
        //mEventManager->beep(event);
        
		event->execute();
    }

    auto eventIter2 = mEvents.begin();
    while(eventIter2 != eventIter)
    {
        event = (*eventIter2);
        auto tmpIter = eventIter2;
        ++eventIter2;
        
        if(event->finished() == true)
        {
            mEvents.erase(tmpIter);
        }
    }
    
//    std::cout << "events scheduler after:\n";
//    std::cout << (*this) << "\n";
//	
//    std::cout << "EventScheduler::processEvents() end\n";
}
