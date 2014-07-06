#include "KAI/KAI.h"

struct CustomProbe
{
};

KAI_BEGIN

class ProbeBase
{
	bool dirty;

public:
	ProbeBase() : dirty(true) {}

	bool IsDirty() const { return dirty; }
	bool IsClean() const { return !dirty; }

	void SetClean() { dirty = false; }
	void SetDirty() { dirty = true; }
};

template <class T>
class Probe : ProbeBase
{
	typedef Type::Traits<T> Traits;
	typedef typename Pointer<T>::PointerType PointerType;
	typedef typename Pointer<T>::ConstPointerType ConstPointerType;
	typedef typename Pointer<T>::Reference Reference;
	typedef typename Pointer<T>::ConstReference ConstReference;
	Pointer<T> ptr;

public:
	Probe(Pointer<T> P) : ptr(P) { }

	Reference Deref()
	{
		SetDirty();
		return *ptr;
	}

	ConstReference ConstDeref() const
	{
		return *ptr;
	}

	PointerType operator->()
	{
		return &Deref();
	}

	ConstPointerType operator->() const
	{
		return &ConstDeref();
	}

	Reference operator*()
	{
		return Deref();
	}

	ConstReference operator*() const
	{
		return ConstDeref();
	}
};

struct MetricsClient : Registry 
{
	typedef nstd::map<Label, Handle> Probes;

private:
	Probes probes;
	//Socket socket;

public:
	template <class T>
	Probe<T> NewProbe(const Label &name)
	{
		Pointer<T> Q = New<T>();
		probes[name] = Q.GetHandle();
		return Q;
	}

	void Update()
	{
		Probes::iterator A = probes.begin(), B = probes.end();
		for (; A != B; ++A)
		{
			//ProbeBase
			//if (!probe->IsDirty())
			//	continue;

			//// TOD: send update

			//probe->SetClean();
		}
	}
};

KAI_END

#include "KAI/BuiltinTypes/Signed32.h"

USING_NAMESPACE_KAI

void TestProbes()
{
	MetricsClient R;
	R.AddClass<int>("int");

//	Probe<int> N = R.NewProbe<int>("Framerate");
}

//EOF

