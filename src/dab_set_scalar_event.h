/** \file dab_set_scalar_event.h
*/

#ifndef _dab_set_scalar_event_h_
#define _dab_set_scalar_event_h_

#include <iostream>
#include <sstream>
#include "ofUtils.h"
#include "dab_event.h"

namespace dab
{

namespace event
{

template<typename ClassType, typename ScalarType>
class SetScalarEvent : public Event
{
public:
    SetScalarEvent( std::shared_ptr<ClassType> pObject, void (ClassType::*pSetFunction)(ScalarType), ScalarType (ClassType::*pGetFunction)() const, ScalarType pValue);
    SetScalarEvent( std::shared_ptr<ClassType> pObject, void (ClassType::*pSetFunction)(ScalarType), ScalarType (ClassType::*pGetFunction)() const, ScalarType pValue, float pTime, float pDuration=0.0, TimeMode pTimeMode=RelativeTime);
    SetScalarEvent(const SetScalarEvent& pEvent);
    SetScalarEvent(double pTime, const SetScalarEvent& pEvent);
    ~SetScalarEvent();
    
    const SetScalarEvent& operator=(const SetScalarEvent& pEvent);
//	SetScalarEvent* copy(); // const = 0;
//  SetScalarEvent* copy( float pTime ); // const = 0;
	
    bool isDerivedClass( Event* pEvent ) const;
    void execute() throw (dab::Exception); // = 0;
    operator std::string() const;
    
    friend std::ostream& operator << ( std::ostream& pOstream, const SetScalarEvent& pEvent)
    {
        pOstream << std::string( pEvent );
        return pOstream;
    };
    
protected:
    std::weak_ptr<ClassType> mTargetObject;
    ScalarType mTargetValue;
    void (ClassType::*mSetFunction)(ScalarType);
    ScalarType (ClassType::*mGetFunction)() const;
    
    ScalarType mStartValue;
    //ScalarType mCurrentValue;
    
    // interpolation stuff for events that run for a certain duration
    ScalarType mInterpolatedValue;
    //float mPreviousExecutionTime;
    float mCurrentExecutionTime;
    //float mCurrentExecutionInterval;
    //float mAverageExecutionInterval = -1.0;
    //float mAverageScale = 0.9;
};
    
template<typename ClassType, typename ScalarType>
    SetScalarEvent<ClassType, ScalarType>::SetScalarEvent( std::shared_ptr<ClassType> pObject, void (ClassType::*pSetFunction)(ScalarType), ScalarType (ClassType::*pGetFunction)() const, ScalarType pValue)
    : mTargetObject(pObject)
    , mSetFunction(pSetFunction)
    , mGetFunction(pGetFunction)
    , mTargetValue(pValue)
{}
  
template<typename ClassType, typename ScalarType>
    SetScalarEvent<ClassType, ScalarType>::SetScalarEvent( std::shared_ptr<ClassType> pObject, void (ClassType::*pSetFunction)(ScalarType), ScalarType (ClassType::*pGetFunction)() const, ScalarType pValue, float pTime, float pDuration, TimeMode pTimeMode)
    : Event(pTime, pDuration, pTimeMode)
    , mTargetObject(pObject)
    , mSetFunction(pSetFunction)
    , mGetFunction(pGetFunction)
    , mTargetValue(pValue)
{}
    
template<typename ClassType, typename ScalarType>
SetScalarEvent<ClassType, ScalarType>::SetScalarEvent(const SetScalarEvent& pEvent)
    : Event(pEvent)
    , mTargetObject(pEvent.mTargetObject)
    , mSetFunction(pEvent.mSetFunction)
    , mGetFunction(pEvent.mGetFunction)
    , mTargetValue(pEvent.mTargetValue)
    , mInterpolatedValue(pEvent.mInterpolatedValue)
    , mCurrentExecutionTime(pEvent.mCurrentExecutionTime)
{}
    
template<typename ClassType, typename ScalarType>
SetScalarEvent<ClassType, ScalarType>::SetScalarEvent(double pTime, const SetScalarEvent& pEvent)
    : Event(pTime, pEvent)
    , mTargetObject(pEvent.mTargetObject)
    , mSetFunction(pEvent.mSetFunction)
    , mGetFunction(pEvent.mGetFunction)
    , mTargetValue(pEvent.mTargetValue)
    , mInterpolatedValue(pEvent.mInterpolatedValue)
    , mCurrentExecutionTime(pEvent.mCurrentExecutionTime)
{}
    
template<typename ClassType, typename ScalarType>
SetScalarEvent<ClassType, ScalarType>::~SetScalarEvent()
{}
    
template<typename ClassType, typename ScalarType>
const SetScalarEvent<ClassType, ScalarType>&
SetScalarEvent<ClassType, ScalarType>::operator=(const SetScalarEvent& pEvent)
{
    Event::operator=(pEvent);
    mTargetObject = pEvent.mTargetObject;
    mSetFunction = pEvent.mSetFunction;
    mGetFunction = pEvent.mGetFunction;
    mTargetValue = pEvent.mTargetValue;
    mInterpolatedValue = pEvent.mInterpolatedValue;
    //mPreviousExecutionTime = pEvent.mPreviousExecutionTime;
    mCurrentExecutionTime = pEvent.mCurrentExecutionTime;
    //mAverageExecutionInterval = pEvent.mAverageExecutionInterval;

    return *this;
}

template<typename ClassType, typename ScalarType>
bool
SetScalarEvent<ClassType, ScalarType>::isDerivedClass( Event* pEvent ) const
{
    return dynamic_cast<SetScalarEvent<ClassType, ScalarType>*>(pEvent) != nullptr;
}
  
template<typename ClassType, typename ScalarType>
void
SetScalarEvent<ClassType, ScalarType>::execute() throw (dab::Exception)
{
    if(mFinished == true) return;
    
    // get target object from weak pointer
    std::shared_ptr<ClassType> targetObject = mTargetObject.lock();
    if(targetObject == nullptr)
    {
        mFinished = true;
        return;
    }
    
    if(mStarted == false && mFinished == false)
    {
        mStartValue = (targetObject.get()->*mGetFunction)();
        mStarted = true;
    }
    
    if(mDuration <= 0.0)
    {
        (targetObject.get()->*mSetFunction)( mTargetValue );
        mFinished = true;
        return;
    }
    
    mCurrentExecutionTime = ofGetElapsedTimef();
    float remainingTime = mDuration - (mCurrentExecutionTime - mTime);
    
    if(remainingTime <= 0.0)
    {
        (targetObject.get()->*mSetFunction)( mTargetValue );
        mFinished = true;
        return;
    }
    
    mInterpolatedValue = mStartValue + (mTargetValue - mStartValue) * (mCurrentExecutionTime - mTime) / mDuration;
    
    (targetObject.get()->*mSetFunction)( mInterpolatedValue );
    
//    mCurrentExecutionInterval = mCurrentExecutionTime - mPreviousExecutionTime;
//    if(mAverageExecutionInterval < 0.0) mAverageExecutionInterval = mCurrentExecutionInterval;
//    else mAverageExecutionInterval = mAverageExecutionInterval * mAverageScale + mCurrentExecutionInterval * (1.0 - mAverageScale);
//        
//    float remainingExecutionCount = remainingTime / mAverageExecutionInterval;
//    mCurrentValue = (targetObject.get()->*mGetFunction)();
//        
//    mInterpolatedValue = mCurrentValue + (mTargetValue - mCurrentValue) / remainingExecutionCount;
//        
//    (targetObject.get()->*mSetFunction)( mInterpolatedValue );
//    
//    mPreviousExecutionTime = mCurrentExecutionTime;
}
    
template<typename ClassType, typename ScalarType>
SetScalarEvent<ClassType, ScalarType>::operator std::string() const
{
    std::stringstream ss;
    
    ss << Event::operator std::string();
    ss << "scalar " << mTargetValue << "\n";
 
    return ss.str();
}
    
};
    
};

#endif
