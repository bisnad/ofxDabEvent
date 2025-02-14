#pragma once

#include "ofMain.h"
#include "dab_event_manager.h"
#include "dab_event_scheduler.h"
#include "dab_event_listener.h"
#include "dab_event.h"

#include <array>
#include <vector>

class EventA : public dab::event::Event
{
public:
	EventA();
	EventA(float pTime, float pDuration = 0.0, dab::event::TimeMode pTimeMode = dab::event::RelativeTime);
	void execute() throw (dab::Exception);

	virtual bool isDerivedClass(Event* pEvent) const;

protected:
};

class EventB : public EventA
{
public:
	EventB();
	EventB(float pTime, float pDuration = 0.0, dab::event::TimeMode pTimeMode = dab::event::RelativeTime);
	void execute() throw (dab::Exception);

	virtual bool isDerivedClass(Event* pEvent) const;

protected:
};

class TestEventListener : public dab::event::EventListener
{
public:
	void notify(std::shared_ptr<dab::event::Event> pEvent);

protected:
};

class ofApp : public ofBaseApp {

public:
	void setup();
	void update();
	void draw();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);

	dab::event::EventManager* mEventManager;
	dab::event::EventScheduler* mEventScheduler;
	std::shared_ptr<TestEventListener> mEventListener;
	std::shared_ptr<TestEventListener> mEventAListener;
	std::shared_ptr<TestEventListener> mEventBListener;

	float getValue() const;
	void setValue(float pValue);
	float mValue = 2.0;

	int getIntValue() const;
	void setIntValue(int pValue);
	int mIntValue = 12;

	const array<float, 2>& getFloatArray() const;
	void setFloatArray(const array<float, 2>& pArray);
	std::array<float, 2> mArray = { 5.0, 5.0 };

	const std::vector<float>& getFloatVector() const;
	void setFloatVector(const std::vector<float>& pVector);
	std::vector<float> mVector = { 5.0, 5.0 };

	std::shared_ptr<ofApp> mSelf;
};
