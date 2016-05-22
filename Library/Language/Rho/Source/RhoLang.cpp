#include <fstream>

#include "KAI/Executor/Executor.h"
#include "KAI/Language/Rho/TauLang.h"

using namespace std;

KAI_BEGIN

void RhoLang::Print()
{
	/*
	cout << "Input--" << endl;
	cout << lex->GetInput() << endl;

	cout << "Lexer--" << endl;
	lex->Print();

	cout << "Parser--" << endl;
	parse->Print();

	cout << "Trans--" << endl;
	cout << trans->Result() << endl;
	 */
}

Pointer<Continuation> RhoLang::TranslateFile(const char *name, Structure st)
{
	ifstream file(name, ios::binary);
	if (!file)
		return Object();

	file.seekg(0, ios::end);
	int len = (int)file.tellg();
	char *text = new char[len + 1];
	file.seekg(0, ios::beg);
	file.read(text, len);
	text[len] = 0;

	return Translate(text, st);
}

Pointer<Continuation> RhoLang::Translate(const char *text, Structure st)
{
	KAI_UNUSED_2(text, st);
	return Object();

	//trans = std::make_shared<Translator>(reg);
	//trans->Translate(text);

	//if (lex->Failed)
	//	Fail(lex->Error);

	//if (parse->Failed)
	//	Fail(parse->Error);

	//if (trans->Failed)
	//	Fail(trans->Error);

	//return !Failed;
}

KAI_END
