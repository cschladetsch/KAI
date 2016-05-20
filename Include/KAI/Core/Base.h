// TODO:
// this is one of the worst files in the project, and shows why

#pragma once

#include <functional>
#include <tuple>
#include <utility>
#include <memory>
#include <utility>
#include <boost/type_index.hpp>

#include <KAI/Core/Config/Base.h>
#include <KAI/Core/FwdDeclarations.h>

KAI_BEGIN

const char *ToLower(const char *str);

	typedef ::std::size_t HashValue;

template <class T>
struct FwdBasePointer { };

KAI_END

