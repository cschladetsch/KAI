#pragma once

#include "KAI/Core/Object/PropertyBase.h"
#include "KAI/Core/Object/Label.h"
#include "KAI/Core/Pointer.h"
#include "KAI/Core/Detail/AccessorDetail.h"

KAI_BEGIN

/// make a property which can only access the given class member
template <class K, class C, class T>
PropertyBase *MakeProperty(T const (C::*F), Label const &N
	, MemberCreateParams::Enum create_params = MemberCreateParams::Default)
{
	return new property_detail::MakeAccessor<K,C,T>(F, N, create_params);
}

/// make a property which can only access the given class member
template <class K, class C, class T>
PropertyBase *MakeProperty(Pointer<const T> const (C::*F), Label const &N
	, MemberCreateParams::Enum create_params = MemberCreateParams::Default)
{
	return new property_detail::MakeAccessor<K,C,T>(F, N, create_params);
}

/// make a property which can access and change the given class member
template <class K, class C, class T>
PropertyBase *MakeProperty(T (C::*F), Label const &N
	, MemberCreateParams::Enum create_params = MemberCreateParams::Default)
{
	return new property_detail::MakeMutator<K,C,T>(F, N, create_params);
}

KAI_END

