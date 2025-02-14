#include "ofApp.h"
#include "dab_event.h"
#include "dab_set_scalar_event.h"
#include "dab_set_array_event.h"

EventA::EventA()
	: Event()
{}

EventA::EventA(float pTime, float pDuration, dab::event::TimeMode pTimeMode)
	: Event(pTime, pDuration, pTimeMode)
{}

bool
EventA::isDerivedClass(Event* pEvent) const
{
	return dynamic_cast<EventA*>(pEvent) != nullptr;
}

void
EventA::execute() throw (dab::Exception)
{
	std::cout << "EventA::execute() \n";
	std::cout << "type " << std::type_index(typeid(this)).name() << " time " << mTime << "\n";

	mFinished = true;
}

EventB::EventB()
	: EventA()
{}

EventB::EventB(float pTime, float pDuration, dab::event::TimeMode pTimeMode)
	: EventA(pTime, pDuration, pTimeMode)
{}

bool
EventB::isDerivedClass(Event* pEvent) const
{
	return dynamic_cast<EventB*>(pEvent) != nullptr;
}

void
EventB::execute() throw (dab::Exception)
{
	std::cout << "EventB::execute() \n";
	std::cout << "type " << std::type_index(typeid(this)).name() << " time " << mTime << "\n";

	mFinished = true;
}

void
TestEventListener::notify(std::shared_ptr<dab::event::Event> pEvent)
{
	std::cout << "Listener notified event time " << pEvent->time() << "\n";
}

template <typename T, typename R, typename ...Args>
R proxycall(T & obj, R(T::*mf)(Args...), Args &&... args)
{
	return (obj.*mf)(std::forward<Args>(args)...);
}

template <typename ClassType, typename ArgType>
void proxySetFunction(ClassType& pObject, void (ClassType::*setFunction)(ArgType), ArgType pArgument)
{
	return (pObject.*setFunction)(pArgument);
}

template <typename ClassType, typename ReturnType>
ReturnType proxyGetFunction(ClassType& pObject, ReturnType(ClassType::*getFunction)() const)
{
	return (pObject.*getFunction)();
}

template<typename ClassType, typename ArgType>
class FunctionTestClass
{
public:
	FunctionTestClass(ClassType* pObject, void (ClassType::*setFunction)(ArgType), ArgType(ClassType::*getFunction)() const, ArgType pValue);

	void test();

protected:
	ClassType* mObject;
	ArgType mValue;

	void (ClassType::*mSetFunction)(ArgType);
	ArgType(ClassType::*mGetFunction)() const;
};

template<typename ClassType, typename ArgType>
FunctionTestClass<ClassType, ArgType>::FunctionTestClass(ClassType* pObject, void (ClassType::*setFunction)(ArgType), ArgType(ClassType::*getFunction)() const, ArgType pValue)
	: mObject(pObject)
	, mSetFunction(setFunction)
	, mGetFunction(getFunction)
	, mValue(pValue)
{}

template<typename ClassType, typename ArgType>
void
FunctionTestClass<ClassType, ArgType>::test()
{
	ArgType currentValue = (mObject->*mGetFunction)();
	(mObject->*mSetFunction)(currentValue + mValue);
}

//--------------------------------------------------------------
void ofApp::setup()
{
	mSelf = std::shared_ptr<ofApp>(this);
	mEventManager = new dab::event::EventManager();
	mEventScheduler = new dab::event::EventScheduler(mEventManager);

	//proxycall<ofApp, void, float>(*this, &ofApp::setValue, 0.5);
	//proxySetFunction<ofApp, float>(*this, &ofApp::setValue, 0.5);
	//proxyGetFunction<ofApp, float>(*this, &ofApp::getValue);

	//    FunctionTestClass<ofApp, float>* myFunctionTestClass = new FunctionTestClass<ofApp, float>( this, &ofApp::setValue, &ofApp::getValue, 5.0 );
	//    myFunctionTestClass->test();

	//    std::shared_ptr< dab::event::SetScalarEvent<ofApp, float> > setScalarEvent = std::shared_ptr< dab::event::SetScalarEvent<ofApp, float> >(new dab::event::SetScalarEvent<ofApp, float>( mSelf,  &ofApp::setValue, &ofApp::getValue, 0.5, 5.0, 4.0 ) );
	//    std::shared_ptr< dab::event::SetScalarEvent<ofApp, int> > setScalarEvent2 = std::shared_ptr< dab::event::SetScalarEvent<ofApp, int> >(new dab::event::SetScalarEvent<ofApp, int>( mSelf,  &ofApp::setIntValue, &ofApp::getIntValue, 20, 5.0, 10.0 ) );


	//dab::event::SetArrayEvent< ofApp, std::array<float,2> >* arrayEvent = new dab::event::SetArrayEvent< ofApp, std::array<float,2> >( mSelf, &ofApp::setFloatArray, &ofApp::getFloatArray, testArray );

	//    std::shared_ptr< dab::event::SetArrayEvent<ofApp, std::array<float, 2> > > setArrayEvent = std::shared_ptr< dab::event::SetArrayEvent<ofApp, std::array<float, 2> > >( new dab::event::SetArrayEvent<ofApp, std::array<float, 2> >( mSelf, &ofApp::setFloatArray, &ofApp::getFloatArray, std::array<float, 2>( { 10.0, 1.0} ), 5.0, 10.0 ) );
	//    mEventScheduler->scheduleEvent(setArrayEvent);

	std::shared_ptr< dab::event::SetArrayEvent<ofApp, std::vector<float> > > setArrayEvent = std::shared_ptr< dab::event::SetArrayEvent<ofApp, std::vector<float> > >(new dab::event::SetArrayEvent<ofApp, std::vector<float> >(mSelf, &ofApp::setFloatVector, &ofApp::getFloatVector, std::vector<float>({ 10.0, 1.0 }), 5.0, 10.0));
	mEventScheduler->scheduleEvent(setArrayEvent);


	//    int eventCount = 4;
	//    for(int eI=0; eI<eventCount; ++eI) mEventScheduler->scheduleEvent( std::shared_ptr<dab::event::Event>( new dab::event::Event( ofRandom(1.0) ) ) );
	//    for(int eI=0; eI<eventCount; ++eI) mEventScheduler->scheduleEvent( std::shared_ptr<EventA>( new EventA( ofRandom(1.0) ) ) );
	//    for(int eI=0; eI<eventCount; ++eI) mEventScheduler->scheduleEvent( std::shared_ptr<EventB>( new EventB( ofRandom(1.0) ) ) );

	//    mEventScheduler->scheduleEvent(setScalarEvent);
	//    //mEventScheduler->scheduleEvent(setScalarEvent2);

	//    mEventListener = std::shared_ptr<TestEventListener>(new TestEventListener());
	//    mEventAListener = std::shared_ptr<TestEventListener>(new TestEventListener());
	//    mEventBListener = std::shared_ptr<TestEventListener>(new TestEventListener());

	//mEventManager->addEventListener<dab::event::Event>(mEventListener);
	//mEventManager->addEventListener<EventA>(mEventAListener);
	//mEventManager->addEventListener<EventB>(mEventBListener);

	double z = 0;
	decltype(z) x;


	//std::shared_ptr< dab::event::SetScalarEvent<ofApp, float> > setScalarEvent( &this->setValue, &this->getValue, 0.5 );


	//ParameterFunction parFunction = &(this->setValue);
}

//--------------------------------------------------------------
void ofApp::update()
{
	mEventScheduler->update();
}

//--------------------------------------------------------------
void ofApp::draw() {

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}

float
ofApp::getValue() const
{
	std::cout << "ofApp getValue " << mValue << "\n";

	return mValue;
}

void
ofApp::setValue(float pValue)
{
	std::cout << "ofApp setValue " << pValue << "\n";

	mValue = pValue;
}

int
ofApp::getIntValue() const
{
	std::cout << "ofApp getIntValue " << mIntValue << "\n";

	return mIntValue;
}

void
ofApp::setIntValue(int pValue)
{
	std::cout << "ofApp setIntValue " << pValue << "\n";

	mIntValue = pValue;
}

const array<float, 2>&
ofApp::getFloatArray() const
{
	std::cout << "get array: ";
	for (int aI = 0; aI<mArray.size(); ++aI)
	{
		std::cout << mArray[aI] << " ";
	}
	std::cout << "\n";

	return mArray;
}

void
ofApp::setFloatArray(const array<float, 2>& pArray)
{
	std::cout << "set array: ";
	for (int aI = 0; aI<pArray.size(); ++aI)
	{
		std::cout << pArray[aI] << " ";
	}
	std::cout << "\n";

	mArray = pArray;
}

const std::vector<float>&
ofApp::getFloatVector() const
{
	std::cout << "get vector: ";
	for (int aI = 0; aI<mVector.size(); ++aI)
	{
		std::cout << mVector[aI] << " ";
	}
	std::cout << "\n";

	return mVector;
}

void
ofApp::setFloatVector(const std::vector<float>& pVector)
{
	std::cout << "set vector: ";
	for (int aI = 0; aI<pVector.size(); ++aI)
	{
		std::cout << pVector[aI] << " ";
	}
	std::cout << "\n";

	mVector = pVector;
}