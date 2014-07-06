
#ifdef KAI_HAVE_PRAGMA_ONCE
#	pragma once
#endif

#ifndef KAI_PROCESS_BASE_H
#	define KAI_PROCESS_BASE_H

KAI_BEGIN

/// \class ProcessBase.
///
/// \brief Base class for all timer objects that receive an Update() call.
///
/// \author Christian Schladetsch.
class ProcessBase
{
	bool paused;

public:
	Event<Handle> OnUpdate;

	ProcessBase(bool P = true) : paused(P) { }
	virtual ~ProcessBase() { }

	bool IsPaused() const { return paused; }
	void Pause(bool P = true) { paused = P; }
	void Unpause() { paused = false; }

	virtual void Update(float time_delta) { KAI_UNUSED(time_delta); }
};

KAI_END

#endif // KAI_PROCESS_BASE_H

//EOF
