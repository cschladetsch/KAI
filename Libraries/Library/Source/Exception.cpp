
#include "KAI/KAI.h"
#include "KAI/BuiltinTypes/Signed32.h"

KAI_BEGIN

namespace Exception 
{
	std::string Base::ToString() const 
	{ 
		StringStream S;
		std::string loc = location.ToString().c_str();
		loc = loc.substr(loc.find_last_of('/') + 1);
		S << loc.c_str() << text << ": ";
		WriteExtendedInformation(S);
		return S.ToString().c_str(); 
	}

	void TypeMismatch::WriteExtendedInformation(StringStream &S) const
	{
		S << "first=" << Type::Number(first).ToString() << ", second=" << Type::Number(second).ToString();
	}

	void UnknownTypeNumber::WriteExtendedInformation(StringStream &S) const
	{
		S << "type=" << type_number;
	}

	void UnknownObject::WriteExtendedInformation(StringStream &S) const
	{
		S << "handle=" << handle.GetValue();
	}

	void ObjectNotFound::WriteExtendedInformation(StringStream &S) const
	{
		S << "label=" << label;
	}

	void NoOperation::WriteExtendedInformation(StringStream &S) const
	{
		S << "type=" << type_number << ", type_property=" << type_property;
	}

	void PacketExtraction::WriteExtendedInformation(StringStream &S) const
	{
		S << "type=" << type_number;
	}

	void OutOfBounds::WriteExtendedInformation(StringStream &S) const
	{
		S << "type=" << type_number << ", index=" << index;
	}

	void PacketInsertion::WriteExtendedInformation(StringStream &S) const
	{
		S << "type=" << type_number;
	}

	void CannotResolve::WriteExtendedInformation(StringStream &S) const
	{
		if (!path.Empty())
			S << "path=" << path;
		else if (!label.Empty())
			S << "label=" << label;
		else
			S << "object=" << object;
	}

	void InvalidPathname::WriteExtendedInformation(StringStream &S) const
	{
		S << "text=" << text;
	}

	void ObjectNotInTree::WriteExtendedInformation(StringStream &S) const
	{
		S << "object=" << object;
	}

	void UnknownMethod::WriteExtendedInformation(StringStream &S) const
	{
		S << "name=" << name << ", class=" << class_name;
	}

	void UnknownHandle::WriteExtendedInformation(StringStream &S) const
	{
		S << "handle=" << handle.GetValue();
	}

	void InvalidStringLiteral::WriteExtendedInformation(StringStream &S) const
	{
		S << "text='" << text << "'";
	}

	void CannotNew::WriteExtendedInformation(StringStream &S) const
	{
		S << "object=" << arg;
	}

	void UnknownKey::WriteExtendedInformation(StringStream &S) const
	{
		S << "key=" << key;
	}

	void NotImplemented::WriteExtendedInformation(StringStream &S) const
	{
		S << "what=" << text;
	}
	void BadIndex::WriteExtendedInformation(StringStream &S) const
	{
		S << "index=" << index;
	}

	void InvalidIdentifier::WriteExtendedInformation(StringStream &S) const
	{
		S << "what='" << what << "'";
	}

	void FileNotFound::WriteExtendedInformation(StringStream &S) const
	{
		S << "filename='" << filename << "'";
	}
	void UnknownProperty::WriteExtendedInformation(StringStream &S) const
	{
		S << "class=" << klass << ", property=" << prop;
	}

}

KAI_END

//EOF
