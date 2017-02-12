# KAI Include

The root of the KAI system.

Will beed to be re-organised so that users can

	#include "KAI/Console.h"		// just get access to REPL Console
	#include "KAI/Registry.h"		// just for object creation and persistence
	#include "KAI/Network.h"		// just for type-safe remoting etc.

and so on.

Don't really want to add a

	#include "KAI/KAI.h"

Except one exists and just includes ***KAI/Executor/Executor.h***. Which to be honest includes most of the entire system anyway.



