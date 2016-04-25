
KAI_BEGIN

class Handle
{
public:
	typedef int Value;

private:
	Value value;

public:
	Handle(Value V = 0) : value(V) { }

	Value GetValue() const { return value; }
	Value NextValue() { return ++value; }

	friend bool operator<(Handle A, Handle B) { return A.value < B.value; }
	friend bool operator==(Handle A, Handle B) { return A.value == B.value; }
	friend bool operator!=(Handle A, Handle B) { return A.value != B.value; }

	static void Register(Registry &);
};

 StringStream &operator<<(StringStream &, Handle);

KAI_END
