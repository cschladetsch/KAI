
void Array::RemoveAt(int index)
{
	if (index < 0 || index >= static_cast<int>(Size()))
	{
		KAI_THROW_1(BadIndex, index);
	}
	objects.erase(begin() + index);
}

Array::iterator Array::Erase(iterator A) 
{ 
	Detach(*A);
	return objects.erase(A); 
}

void Array::Append(Object const &Q)
{ 
	if (Attach(Q))
		objects.push_back(Q); 
}

Object Array::PopBack()
{ 
	Object Q = objects.back(); 
	Detach(Q);
	objects.pop_back(); 
	return Q; 
}

void Array::Clear()
{ 
	iterator A = begin(), B = end();
	for (; A != B; ++A)
		Detach(*A);
	objects.clear(); 
}

void Array::Erase(Object const &Q)
{
	Erase(Q.GetHandle());
}

void Array::Erase(Handle H)
{
	iterator A = objects.begin(), B = objects.end();
	for (; A != B; ++A)
	{
		if (A->GetHandle() == H)
		{
			Erase(A);
			return;
		}
	}
}

StringStream &operator<<(StringStream &S, const Array &A)
{
	S << "<";
	Array::const_iterator B = A.Begin(), C = A.End();
	String sep = "";
	for (; B != C; ++B)
	{
		S << sep << *B;
		sep = ", ";
	}
	return S << ">";
}


BinaryStream &operator<<(BinaryStream &S, const Array &Q)
{
	int length = Q.Size();
	S << length;
	Array::const_iterator A = Q.Begin(), B = Q.End();
	for (; A != B; ++A)
		S << *A;
	return S;
}

BinaryPacket &operator>>(BinaryPacket &S, Array &Q)
{
	Q.Clear();
	int length = 0;
	S >> length;
	for (int N = 0; N < length; ++N)
	{
		Object R;
		S >> R;
		Q.Append(R);
	}
	return S;
}

void Array::Register(Registry &R)
{
	void (Array::*remove_method)(Object) = &Array::Erase2;

	ClassBuilder<Array>(R, Type::Traits<Array>::Name)
		.Methods
			("Append", &Array::Append2, "Add an Object to the end")
			("Erase", remove_method, "Remove an object from the sequence")
			("RemoveAt", &Array::RemoveAt, "Remove the object at the given index from the sequence")
			("PopBack", &Array::PopBack, "Remove last object from the sequence")
			("Size", &Array::Size, "Get the size")
			("Empty", &Array::Empty, "Returns True if Array has no Objects")
			("At", &Array::At, "Returns the Object at the given Signed32 index")
			("Clear", &Array::Clear, "Removes all Objects from the Array")
		;
}

//EOF
