/** \file dab_set_array_event.h
*/

#ifndef _dab_set_array_event_h_
#define _dab_set_array_event_h_

#include <iostream>

#include <iostream>
#include <sstream>
#include "ofUtils.h"
#include "dab_event.h"

namespace dab
{
    
    namespace event
    {
        
        template<typename ClassType, typename ArrayType>
        class SetArrayEvent : public Event
        {
        public:
            SetArrayEvent( std::shared_ptr<ClassType> pObject, void (ClassType::*pSetFunction)(const ArrayType&), const ArrayType& (ClassType::*pGetFunction)() const, const ArrayType& pValue);
            SetArrayEvent( std::shared_ptr<ClassType> pObject, void (ClassType::*pSetFunction)(const ArrayType&), const ArrayType& (ClassType::*pGetFunction)() const, const ArrayType& pValue, float pTime, float pDuration=0.0, TimeMode pTimeMode=RelativeTime);
            SetArrayEvent(const SetArrayEvent& pEvent);
            SetArrayEvent(double pTime, const SetArrayEvent& pEvent);
            ~SetArrayEvent();
            
            const SetArrayEvent& operator=(const SetArrayEvent& pEvent);
            //	SetScalarEvent* copy(); // const = 0;
            //  SetScalarEvent* copy( float pTime ); // const = 0;
            
            bool isDerivedClass( Event* pEvent ) const;
            void execute() throw (dab::Exception); // = 0;
            operator std::string() const;
            
            friend std::ostream& operator << ( std::ostream& pOstream, const SetArrayEvent& pEvent)
            {
                pOstream << std::string( pEvent );
                return pOstream;
            };
            
        protected:
            std::weak_ptr<ClassType> mTargetObject;
            ArrayType mTargetValue;
            void (ClassType::*mSetFunction)(const ArrayType&);
            const ArrayType& (ClassType::*mGetFunction)() const;
            
            ArrayType mStartValue;
            ArrayType mCurrentValue;
            ArrayType mInterpolatedValue;
            float mCurrentExecutionTime;
        };
        
        template<typename ClassType, typename ArrayType>
        SetArrayEvent<ClassType, ArrayType>::SetArrayEvent( std::shared_ptr<ClassType> pObject, void (ClassType::*pSetFunction)(const ArrayType&), const ArrayType& (ClassType::*pGetFunction)() const, const ArrayType& pValue)
        : mTargetObject(pObject)
        , mSetFunction(pSetFunction)
        , mGetFunction(pGetFunction)
        , mTargetValue(pValue)
        {}
        
        template<typename ClassType, typename ArrayType>
        SetArrayEvent<ClassType, ArrayType>::SetArrayEvent( std::shared_ptr<ClassType> pObject, void (ClassType::*pSetFunction)(const ArrayType&), const ArrayType& (ClassType::*pGetFunction)() const, const ArrayType& pValue, float pTime, float pDuration, TimeMode pTimeMode)
        : Event(pTime, pDuration, pTimeMode)
        , mTargetObject(pObject)
        , mSetFunction(pSetFunction)
        , mGetFunction(pGetFunction)
        , mTargetValue(pValue)
        {}
        
        template<typename ClassType, typename ArrayType>
        SetArrayEvent<ClassType, ArrayType>::SetArrayEvent(const SetArrayEvent& pEvent)
        : Event(pEvent)
        , mTargetObject(pEvent.mTargetObject)
        , mSetFunction(pEvent.mSetFunction)
        , mGetFunction(pEvent.mGetFunction)
        , mTargetValue(pEvent.mTargetValue)
        , mInterpolatedValue(pEvent.mInterpolatedValue)
        , mCurrentExecutionTime(pEvent.mCurrentExecutionTime)
        {}
        
        template<typename ClassType, typename ArrayType>
        SetArrayEvent<ClassType, ArrayType>::SetArrayEvent(double pTime, const SetArrayEvent& pEvent)
        : Event(pTime, pEvent)
        , mTargetObject(pEvent.mTargetObject)
        , mSetFunction(pEvent.mSetFunction)
        , mGetFunction(pEvent.mGetFunction)
        , mTargetValue(pEvent.mTargetValue)
        , mInterpolatedValue(pEvent.mInterpolatedValue)
        , mCurrentExecutionTime(pEvent.mCurrentExecutionTime)
        {}
        
        template<typename ClassType, typename ArrayType>
        SetArrayEvent<ClassType, ArrayType>::~SetArrayEvent()
        {}
        
        template<typename ClassType, typename ArrayType>
        const SetArrayEvent<ClassType, ArrayType>&
        SetArrayEvent<ClassType, ArrayType>::operator=(const SetArrayEvent& pEvent)
        {
            Event::operator=(pEvent);
            mTargetObject = pEvent.mTargetObject;
            mSetFunction = pEvent.mSetFunction;
            mGetFunction = pEvent.mGetFunction;
            mTargetValue = pEvent.mTargetValue;
            mInterpolatedValue = pEvent.mInterpolatedValue;
            mCurrentExecutionTime = pEvent.mCurrentExecutionTime;
            
            return *this;
        }
        
        template<typename ClassType, typename ArrayType>
        bool
        SetArrayEvent<ClassType, ArrayType>::isDerivedClass( Event* pEvent ) const
        {
            return dynamic_cast<SetArrayEvent<ClassType, ArrayType>*>(pEvent) != nullptr;
        }
        
        template<typename ClassType, typename ArrayType>
        void
        SetArrayEvent<ClassType, ArrayType>::execute() throw (dab::Exception)
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
                mInterpolatedValue = mStartValue;
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
            
            float valueScale = (mCurrentExecutionTime - mTime) / mDuration;
            
            int arraySize = mTargetValue.size();
            for(int aI=0; aI<arraySize; ++aI)
            {
                mInterpolatedValue[aI] = mStartValue[aI] + (mTargetValue[aI] - mStartValue[aI]) * valueScale;
            }
            
            
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
        
        template<typename ClassType, typename ArrayType>
        SetArrayEvent<ClassType, ArrayType>::operator std::string() const
        {
            std::stringstream ss;
            
            ss << Event::operator std::string();
            ss << "array ";
            
            int arraySize = mTargetValue.size();
            for(int aI=0; aI<arraySize; ++aI)
            {
                ss << mTargetValue[aI] << " ";
            }
            ss << "\n";
            
            return ss.str();
        }
        
    };
    
};


#endif
