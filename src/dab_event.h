/** \file dab_event.h
*/

#ifndef _dab_event_h_
#define _dab_event_h_

#include <iostream>
#include <typeinfo>
#include <typeindex>
#include "dab_exception.h"

namespace dab
{

namespace event
{

enum TimeMode
{
    RelativeTime,
    AbsoluteTime
};
    
class Event
{
public:
    Event();
    Event(float pTime, float pDuration=0.0, TimeMode pTimeMode=RelativeTime);
    Event(const Event& pEvent);
    Event(double pTime, const Event& pEvent);
    virtual ~Event();
    
    virtual const Event& operator=(const Event& pEvent);
	virtual Event* copy(); // const = 0;
	virtual Event* copy( float pTime ); // const = 0;
	
    virtual bool isDerivedClass( Event* pEvent ) const;
    float time() const;
    void setTime(float pTime, TimeMode pTimeMode=RelativeTime );
    TimeMode timeMode() const;
    float duration() const;
    
    bool started() const;
    bool finished() const;
    
    virtual void execute() throw (dab::Exception); // = 0;
    
    operator std::string() const;
    
    friend std::ostream& operator << ( std::ostream& pOstream, const Event& pEvent)
    {
        pOstream << std::string( pEvent );
        return pOstream;
    };
    
protected:
    TimeMode mTimeMode; ///\brief time mode
	float mTime; ///\brief absolute time (in seconds) since program start, this event is going to happen
	float mDuration; ///\brief duration (in seconds), over which the event takes place
	bool mStarted; ///\brief event started flag
	bool mFinished; ///\brief event completion flag
};
    
};
    
};

#endif
