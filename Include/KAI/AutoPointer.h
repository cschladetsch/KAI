#pragma once

KAI_BEGIN

template <class T>
struct AutoPointer
{
private:
	T *ptr;

public:
	AutoPointer(T *P = 0) : ptr(P) { }
	~AutoPointer() { delete P; }

	T *operator->() { return P; }
	T &operator*() { return *P; }

private:
	AutoPointer(const AutoPointer<T> &);
	AutoPointer<T> &operator=(const AutoPointer<T> &);
};

KAI_END
