/** \file dab_event.cpp
*/

#include "dab_event.h"
#include "ofUtils.h"

using namespace dab;
using namespace dab::event;

Event::Event()
: mTime(0.0)
, mDuration(0.0)
, mTimeMode(RelativeTime)
, mStarted(false)
, mFinished(false)
{}
    
Event::Event(float pTime, float pDuration, TimeMode pTimeMode)
: mTime(pTime)
, mDuration(pDuration)
, mTimeMode(pTimeMode)
, mStarted(false)
, mFinished(false)
{
	if(mTimeMode == RelativeTime) mTime += ofGetElapsedTimef();
}

Event::Event(const Event& pEvent)
: mTime(pEvent.mTime)
, mDuration(pEvent.mDuration)
, mTimeMode(pEvent.mTimeMode)
, mStarted(pEvent.mStarted)
, mFinished(pEvent.mFinished)
{}

Event::Event(double pTime, const Event& pEvent)
: mTime(pTime)
, mDuration(pEvent.mDuration)
, mTimeMode(pEvent.mTimeMode)
, mStarted(false)
, mFinished(false)
{
    if(mTimeMode == RelativeTime) mTime += ofGetElapsedTimef();
}

Event::~Event()
{}
    
const Event&
Event::operator=(const Event& pEvent)
{
    mTime = pEvent.mTime;
    mDuration = pEvent.mDuration;
    mTimeMode = pEvent.mTimeMode;
    mStarted = pEvent.mStarted;
    mFinished = pEvent.mFinished;
    
    return *this;
}

Event*
Event::copy()
{
    return new Event(*this);
}

Event*
Event::copy( float pTime )
{
    return new Event(pTime, *this);
}

bool
Event::isDerivedClass( Event* pEvent ) const
{
    return dynamic_cast<Event*>(pEvent) != nullptr;
}

float
Event::time() const
{
    return mTime;
}

void
Event::setTime(float pTime, TimeMode pTimeMode)
{
    mTime = pTime;
    mTimeMode = pTimeMode;
    
    if(mTimeMode == RelativeTime) mTime += ofGetElapsedTimef();
}

TimeMode
Event::timeMode() const
{
    return mTimeMode;
}

float
Event::duration() const
{
	return mDuration;
}

bool
Event::started() const
{
    return mStarted;
}

bool
Event::finished() const
{
    return mFinished;
}

void
Event::execute() throw (dab::Exception)
{
    std::cout << "Event::execute() \n";
    std::cout << "type " << std::type_index(typeid(this)).name() << " time " << mTime << "\n";
    
    mFinished = true;
}

Event::operator std::string() const
{
    std::stringstream ss;
    
    ss << "Event: type " << std::type_index(typeid(this)).name() << " time: " << mTime << " duration: " << mDuration << " started: " << mStarted << " finished: " << mFinished << "\n";
    return ss.str();
}