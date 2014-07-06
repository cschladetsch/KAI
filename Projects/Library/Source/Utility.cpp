
#include "KAI/KAI.h"
#include "KAI/Operation.h"
#include "KAI/BuiltinTypes/Array.h"
#include "KAI/Continuation.h"
#include "KAI/Value.h"
#include "KAI/Compiler.h"
#include "KAI/Reflected.h"
#include "KAI/Executor.h"
#include <iostream>
#include <fstream>

KAI_BEGIN

namespace debug
{
	Trace::~Trace()
	{
		const char *type_str = "";
		switch (type)
		{
		case Warn: type_str = "Warning: "; break;
		case Error: type_str = "Error: "; break;
		case Fatal: type_str = "Fatal: "; break;
		}
		String text = (file_location.ToString(false) + type_str + ToString());
		text.RemoveAll("sif::sh::");
		DebugTrace(text.c_str());
	}

	StringStream &Trace::operator<<(const Object& X)
	{
		static_cast<StringStream &>(*this) << X.GetHandle() << ": " << X;
		return *this;
	}
}

StringStream& operator<<(StringStream& S, ObjectColor::Color C)
{
	switch (C)
	{
	case ObjectColor::White:
		return S << "White";
	case ObjectColor::Grey:
		return S << "Grey";
	case ObjectColor::Black:
		return S << "Black";
	}
	return S << "UnknownColor";
}

bool ExecuteFile(const char *filename, Pointer<Executor> executor, Pointer<Compiler> compiler, Object scope)
{
	std::fstream file(filename, std::ios::in);
	if (!file)
		return false;
	char line[2000];
	StringStream text;
	while (file.getline(line, 2000))
	{
		text.Append(line);
		text.Append('\n');
	}
	try
	{
		Pointer<Continuation> cont = compiler->Compile(*executor->Self->GetRegistry(), text.ToString());
		cont->SetScope(scope);
		executor->Continue(cont);
		return true;
	}
	catch (Exception::Base &E)
	{
		std::cerr << "Exception running file '" << filename << "': " << E.ToString() << std::endl;
	}
	catch (std::exception &E2)
	{
		std::cerr << "Exception running file '" << filename << "': " << E2.what() << std::endl;
	}
	catch (...)
	{
		std::cerr << "Exception running file '" << filename << "'" << std::endl;
	}
	return false;
}

void ToStringStream(const Object &Q, StringStream &S, int level)
{
	StringStream indent;
	for (int N = 0; N < level; ++N)
	{
		indent.Append(' ');
		indent.Append(' ');
	}

	S << indent.ToString();
	if (!Q.Valid())
	{
		S << "InvalidObject\n";
		return;
	}

	const StorageBase &base = Q.GetStorageBase();
	bool write_header = false;
	if (write_header)
	{
		S << base.GetLabel().ToString() << ": ";
	}
	if (Q.GetClass()->HasTraitsProperty(Type::Properties::StringStreamInsert))
	{
		Q.GetClass()->Insert(S, base);
	}
	S << "\n";

	foreach (Dictionary::value_type const &child, base.GetDictionary())
	{
		ToStringStream(child.second, S, level + 1);
	}
	return;
}

void ToXmlStream(const Object &Q, StringStream &S, int level)
{
	StringStream indent;
	for (int N = 0; N < level; ++N)
	{
		indent.Append(' ');
		indent.Append(' ');
	}

	if (!Q.Valid())
	{
		//S << "<!!INVALID!!>\n";
		return;
	}
	StorageBase const &base = Q.GetStorageBase();
	ClassBase const &klass = *Q.GetClass();
	S << indent.ToString() << "<Object type='" << klass.GetName() 
		<< "' name='" << base.GetLabel().ToString() 
		//<< "' handle='" << (int)Q.GetHandle().GetValue() 
		<< "'>\n";
	if (Q.GetClass()->HasTraitsProperty(Type::Properties::StringStreamInsert))
	{
		S << indent.ToString() << "  <Value>";
		if (klass.HasOperation(Type::Properties::StringStreamInsert))
			klass.Insert(S, base);
		S << "</Value>\n";
	}

	foreach (ClassBase::Properties::value_type const &prop_iter, klass.GetProperties())
	{
		PropertyBase const &property = *prop_iter.second;
		S << indent.ToString() <<"<Property name='" << property.GetFieldName() << "'>";
		//ToXmlStream(child.second, S, level + 1);
		S << property.GetValue(base);
		S << "</Property>\n";
	}

	const Dictionary &dict = base.GetDictionary();
	foreach (Dictionary::value_type const &child, dict)
	{
		ToXmlStream(child.second, S, level + 1);
	}
	S << indent.ToString() << "</Object>\n";
	return;
}


KAI_END

//EOF
