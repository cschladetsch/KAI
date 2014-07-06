#include "KAI/KAI.h"
#include <fstream>
#include <iostream>

#include "KAI/Translator/RhoLang.h"
#include "KAI/Translator/Parser.h"
#include "KAI/Translator/Translator.h"

#include "KAI/Executor.h"

using namespace std;

KAI_BEGIN

RhoLang::RhoLang(Registry &r)
	: reg(r)
{
}

void RhoLang::Print()
{
	cout << "Input--" << endl;
	cout << lex->input << endl;

	cout << "Lexer--" << endl;
	lex->Print();

	cout << "Parser--" << endl;
	parse->Print();

	cout << "Trans--" << endl;
	cout << trans->Result() << endl;
}


bool RhoLang::TranslateFile(const char *name, Parser::Structure st)
{
	ifstream file(name, ios::binary);
	if (!file)
		return false;
	file.seekg(0, ios::end);
	int len = (int)file.tellg();
	char *text = new char[len + 1];
	file.seekg(0, ios::beg);
	file.read(text, len);
	text[len] = 0;

	return Translate(text, st);
}

bool RhoLang::Translate(const char *text, Parser::Structure st)
{
	lex = std::make_shared<Lexer>(text);
	parse = std::make_shared<Parser>(lex, st);
	trans = std::make_shared<Translator>(parse, reg);
	if (lex->Failed)
		return Fail(lex->Error);
	if (parse->Failed)
		return Fail(parse->Error);
	if (trans->Failed)
		return Fail(trans->Error);
	return true;
}

KAI_END
