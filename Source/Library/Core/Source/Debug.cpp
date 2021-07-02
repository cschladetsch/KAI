#include <iostream>

#include "rang.hpp"

#include <KAI/Core/Config/Base.h>
#include <KAI/Core/Object/Object.h>
#include <KAI/Core/Exception.h>
#include <KAI/Core/Debug.h>
#include <KAI/Console/ConsoleColor.h>

using namespace std;

KAI_BEGIN

StringStream& operator<<(StringStream &s, const Structure & st)
{
    KAI_NOT_IMPLEMENTED();
    return s;
}

// this is just being used while I (re)sort out the cross-platform console color issues
ostream& operator<<(ostream &S, ConsoleColor::EType type)
{
    return S;
}

namespace debug
{
    bool Trace::TraceFileLocation = false;
    bool Trace::StripPath = true;
    bool Trace::TraceFunction = false;

    void MaxTrace()
    {
        Trace::TraceFileLocation = true;
        Trace::StripPath = false;
        Trace::TraceFunction = true;
    }

    void MinTrace()
    {
        Trace::TraceFileLocation = true;
        Trace::StripPath = true;
        Trace::TraceFunction = true;
    }
    StringStream& Trace::operator<<(Object const&obj)
    {
        return *this << obj.ToString();
    }

    const char *TypeToString(Trace::Type type)
    {
        switch (type)
        {
            #define CASE(V) case Trace::V: return #V;
            CASE(Warn);
            CASE(Error);
            CASE(Fatal);
            case Trace::Information: return "Info";
        }

        return "??";
    }

    Trace::~Trace()
    {
        const auto filelocCol = rang::fg::gray;
        const auto textCol = rang::fg::yellow;
        const auto val = ToString();

        if (TraceFileLocation)
            cout << filelocCol << file_location.ToString().c_str();

        cout << TypeToString(type) << ": " << textCol << val.c_str() << endl;
    }
}

KAI_END
