
KAI_BEGIN

namespace Bin
{
	Object GetMethods(Object Q);

	void Help();

	Object GetProperties(Object Q);

	Vector3 ScaleVector3(Vector3 vec, float scalar);
	Vector3 AddVector3(Vector3 vec, Vector3 addition);
	void WriteToFile(String filename, Object Q);
	String ReadFile(String filename);
	void Printf(String fmt, Array items);
	void Print(Object Q);
	void PrintXml(Object Q);
	Object UpCast(Object Q);
	void SetClean(Object Q, bool D);
	bool IsDirty(Object Q);
	bool IsClean(Object Q);
	bool IsConst(Object Q);
	void Assert(bool B);
	Object Freeze(Object Q);
	Object Thaw(Object Q);
	String ToString(Object Q);
	String ToXmlString(Object Q);
	void ExitToOS(int N);

	Object RunOne(Object object);
	Object RunAllTests(Object object);

	Object TypeNumberToClass(Object tn);
	Object Describe(Object Q);
	String info(Object object);
	void AddFunctions(Object Q);


	







}

KAI_END

