## ofxDabEvent

**Author**: Daniel Bisig - Coventry University, UK - [ad5041@coventry.ac.uk](ad5041@coventry.ac.uk) - Zurich University of the Arts, CH - [daniel.bisig@zhdk.ch](daniel.bisig@zhdk.ch)

**Dependencies**: [ofxDabBase](https://bitbucket.org/dbisig/ofxdabbase_011/src/master/)

---

## Summary

ofxDabEvent provides a set of classes for handling events. Events can perform an operation at a set time either instantaneously or over a duration. The event class needs to be subclassed to execute a specific operation. The code is compatible with OpenFrameworks 0.11 and has been tested on Windows and MacOS. The following classes are available.



**Event**: Base class for events that can be executed at a particular time.

**EventScheduler**: handles the execution of events as well as the removal of events that finished executing.

**EventListener**: a base class for being notified when an event has been executed.

**EventManager**: keeps track of all events and handles the addition and removal of event listeners

**SetScalarEvent**: a simple example event class that modifies scalar values

**SetArrayEvent**: a simple example event class that modifies arrays of values