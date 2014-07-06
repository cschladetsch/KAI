

#include "KAI/KAI.h"
#include "KAI/Operation.h"
#include "KAI/Value.h"
#include "KAI/Continuation.h"
#include "KAI/Compiler.h"
#include "KAI/Reflected.h"
#include "KAI/Executor.h"
#include "KAI/Pathname.h"

#include <iostream>
#include <algorithm>
#include <assert.h>

KAI_BEGIN

#include "CompilerTokens.inl"

bool Compiler::Destroy()
{
	StringToOperation::iterator A = string_to_op.begin(), B = string_to_op.end();
	for (; A != B; ++A)
		A->second.SetManaged(true);
	return true;
}

void Compiler::AddOperation(int N, const String &S)
{
	Operation T = Operation::Type(N);
	Pointer<Operation> Q = Self->New(T);
	Q.SetManaged(false);
	string_to_op[S] = Q;
	op_to_string[T] = S;
}

Compiler::Token Compiler::NextToken(String::const_iterator A, String::const_iterator B) const
{
	Token token(A,B);

	while (A != B && isspace(*A)) 
		++A;
	
	if (A == B)
		return token;
	
	token.first = A;

	if (*token.first == '"')
	{
		if (++A == B)
			KAI_THROW_1(InvalidStringLiteral, String(token.first,B));
		while (A != B)
		{
			if (*A == '\\')
			{
				if (++A == B)
					KAI_THROW_1(InvalidStringLiteral, String(token.first,B));
			}
			else if (*A == '"')
				break;
			++A;
		}
		if (A == B || *A != '"')
			KAI_THROW_1(InvalidStringLiteral, String(token.first,B));
		token.type = Token::StringLiteral;
		token.second = ++A;
	}
	else if (token.first[0] == '/' && token.first + 1 != token.second && token.first[1] == '/')
	{
		while (A != B && *A != '\n')
			++A;
		token.type = Token::Comment;
		token.second = A;
	}
	else
	{
		while (A != B && !isspace(*A))// && A[0] != '/')//HACK! && (A + 1 != B && A[1] != '/')))
			++A;
		token.type = Token::Word;
		token.second = A;
	}
	return token;
}

Pointer<Continuation> Compiler::Compile(Registry &R, const String &text) const
{
	Tokens tokens;
	Tokenise(text, tokens);
	Pointer<Continuation> cont = Parse(R, tokens);
	cont->SetSourceCode(text);
	//cont->Enter();
	return cont;
}

void Compiler::Tokenise(const String &text, Tokens &tokens) const
{
	if (text.empty())
		return;
	Token range(text.begin(), text.end());
	while (range.first != range.second)
	{
		Token token = NextToken(range.first, range.second);
		if (token.first == token.second)
			break;
		range.first = token.second;
		tokens.push_back(token);
	}
}

bool Contains(const String &text, const char *find)
{
	while (*find)
	{
		if (std::find(text.begin(), text.end(), *find) != text.end())
			return true;
		++find;
	}
	return false;
}

String MakeString(const String& raw_string)
{
	StringStream S;
	String::const_iterator A = raw_string.begin(), B = raw_string.end();
	++A;	// skip over leading "
	--B;	// dont go to trailing "
	for (; A != B; ++A)
	{
		if (*A == '\\')
			++A;
		S.Append(*A);
	}
	return S.ToString();
}

Pointer<Continuation> Compiler::Parse(Registry &R, const Tokens &tokens) const
{
	Tokens::const_iterator begin = tokens.begin();
	return ParseContinuation(R, begin, tokens.end());
}

Pointer<Continuation> Compiler::ParseContinuation(Registry &R, Tokens::const_iterator &A, Tokens::const_iterator B) const
{
	Pointer<Continuation> cont = R.New<Continuation>();
	Pointer<Array> code = R.New<Array>();
	cont->SetCode(code);

	std::locale loc;
	for (; A != B;)
	{
		const Token &token = *A;
		String str_token(token.first, token.second);
		switch (token.type)
		{
		case Token::Comment:
			break;
		case Token::Word:
			{
				StringToOperation::const_iterator op = string_to_op.find(str_token.c_str());
				if (op != string_to_op.end())
				{
					if (*op->second == Operation::ContinuationEnd)
					{
						++A;
						return cont;
					}
					if (*op->second == Operation::ContinuationBegin)
					{
						++A;
						code->Append(ParseContinuation(R, A, B));
						continue;
					}
					code->Append(op->second);
					break;
				}
				if (isdigit(*token.first) || *token.first == '-')
				{
					if (std::find(token.first, token.second, '.') != token.second)
						code->Append(R.New((float)atof(str_token.c_str())));
					else
						code->Append(R.New(atoi(str_token.c_str())));
					break;
				}

				if (Contains(str_token, Pathname::Literals::AllButQuote))
				{
					Value<Pathname> P = R.New<Pathname>();
					P->FromString(str_token);
					code->Append(P.GetObject());
					break;
				}

				// default to treating as a label
				Value<Label> L = R.New<Label>();
				L->FromString(str_token);
				code->Append(L.GetObject());
			}
			break;
		case Token::StringLiteral:
			code->Append(R.New<String>(MakeString(str_token)));
			break;
		}
		++A;
	}
	return cont;
}

Object Compiler::Compile(String const &text) const
{
	return Compile(*Self->GetRegistry(), text);
}

Object Compiler::Compile2(String text) const
{
	return Compile(*Self->GetRegistry(), text);
}

void Compiler::Register(Registry &R, const char *name)
{
	Object (Compiler::*Compile)(String) const = &Compiler::Compile2;
	ClassBuilder<Compiler>(R, name)
		.Methods
		("Compile", Compile)
		;
}

StringStream &operator<<(StringStream &S, Operation const &P)
{
	return S << P.ToString();
}

StringStream &operator>>(StringStream &, Operation &)
{
	KAI_NOT_IMPLEMENTED();
}

// TODO: use char for operation::value

BinaryStream &operator<<(BinaryStream &S, Operation const &P)
{
	return S << (int)(P.GetTypeNumber());
}

BinaryPacket &operator>>(BinaryPacket &S, Operation &P)
{
	int val;
	S >> val;
	P.SetType(Operation::Type(val));
	return S;
}

#include "OperationToString.inl"

const char *Operation::ToString() const
{
	return ToString((int)value);
}

void Operation::Register(Registry &R)
{
	ClassBuilder<Operation>(R, "Operation")
		;
}

KAI_END

//EOF
