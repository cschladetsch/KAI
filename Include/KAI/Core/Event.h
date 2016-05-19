#pragma once

#include <KAI/Language/Tau/Tau.h>
#include "KAI/Core/BuiltinTypes/Void.h"

// DESCRIPTION
// This file contains a definition for a multi-cast Event.
//
// An Event is a message sent by an object to signal 
// the occurrance of an action. There may be many receivers
// for the event. Event receivers (also called Listeners or Observers)
// register thier interest in an event by adding a delegate
// to the event:
//
// EXAMPLE
//		void foo(int, string);
//		struct Bar { void spam(int, string); };
//
//		Bar bar;
//		Event<int, string> event;				// create a new event which takes int and string args
//		event += foo;							// add a callback to foo function
//		event += pair(&bar, &Bar::spam);		// add a callback to a bound method
//		event(42, "hello");						// invoke the event. foo and bar.spam will both be called
//		event -= foo;							// remove foo function from delegate list
//		event(12, ", world");					// now just bar.spam will be called
//
// NOTES
// Return types for event delegates must return void.
// Currently supports events with up to three (3) arguments.
//
// REVISION
//		November 2007		@author christian		created
//		June 2008			@author christian		added SystemEvent

KAI_BEGIN

#	include "KAI/Core/Detail/Event.h"
#	include "KAI/Core/Detail/SystemEvent.h"

template <class T0 = event_detail::Null, class T1 = event_detail::Null, class T2 = event_detail::Null>
struct Event : event_detail::EventType<event_detail::Arity<T0, T1, T2>:: Value>::template Create<T0, T1, T2> { };

template <class T0 = event_detail::Null, class T1 = event_detail::Null, class T2 = event_detail::Null>
struct SystemEvent : event_detail_sys::EventType<event_detail::Arity<T0, T1, T2>::Value>::template Create<T0, T1, T2> { };


KAI_END

#endif // KAI_EVENT_H

//EOF
