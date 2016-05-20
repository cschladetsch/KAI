#pragma once

KAI_EGIN

template <class T>
struct Argument
{
	typedef const T &Type;
};

template <class T>
struct Argument<const T>
{
	typedef const T &Type;
};

template <class T>
struct Argument<T &>
{
	typedef T &Type;
};

template <class T>
struct Argument<const T&>
{
	typedef const T &Type;
};

