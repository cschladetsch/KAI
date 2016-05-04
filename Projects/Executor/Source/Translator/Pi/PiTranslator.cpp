#include "KAI/ExecutorPCH.h"
#include "KAI/Translator/Pi/PiTranslator.h"

#include <boost/lexical_cast.hpp>

KAI_BEGIN

void PiTranslator::TranslateNode(AstNodePtr node)
{
	switch (node->type)
	{
		case PiAstNodeEnumType::Continuation:
		{
			PushNew();
			for (auto const &ch : node->Children)
			{
				TranslateNode(ch);
			}
			Append(Pop());
		}
		case PiAstNodeEnumType::Array:
		{
			PushNew();
			for (auto const &ch : node->Children)
			{
				TranslateNode(ch);
			}
			Append(Pop());
		default:
			AppendSingle(node->token);
			break;
		}
	}
}

//Pointer<Continuation> PiTranslator::Translate(const char *text, Structure st)
//{
//	if (text == 0 || text[0] == 0)
//		return Object();
//
//	auto lex = std::make_shared<PiLexer>(text);
//	lex->Process();
//	if (lex->GetTokens().empty())
//		return Object();
//	if (lex->Failed)
//		Fail(lex->Error);
//
//	auto parse = std::make_shared<Parser>(lex);
//	parse->Process(st);
//	if (parse->Failed)
//		Fail(parse->Error);
//
//	Run(parse);
//
//	if (stack.empty())
//		KAI_THROW_0(EmptyStack);
//
//	return stack.back();
//}
//
void PiTranslator::AppendSingle(const TokenNode& tok)
{
	switch (tok.type)
	{
	case PiTokenEnumType::String:
		AppendNew(String("\"" + tok.Text() + "\""));
		break;

	case PiTokenEnumType::Bool:
		AppendNew(boost::lexical_cast<bool>(tok.Text()));
		break;

	case PiTokenEnumType::Int:
		AppendNew(boost::lexical_cast<int>(tok.Text()));
		break;

	case PiTokenEnumType::Drop:
		AppendOp(Operation::Drop);
		break;

	case PiTokenEnumType::Dup:
		AppendOp(Operation::Dup);
		break;

	case PiTokenEnumType::Plus:
		AppendOp(Operation::Plus);
		break;

	case PiTokenEnumType::Minus:
		AppendOp(Operation::Minus);
		break;

	case PiTokenEnumType::Mul:
		AppendOp(Operation::Multiply);
		break;

	case PiTokenEnumType::Equiv:
		AppendNew(Operation::Equiv);
		break;

	case PiTokenEnumType::Greater:
		AppendNew(Operation::Greater);
		break;

	case PiTokenEnumType::Rot:
		AppendNew(Operation::Rot);
		break;

	case PiTokenEnumType::PickN:
		AppendNew(Operation::Pick);
		break;

	case PiTokenEnumType::Clear:
		AppendNew(Operation::Clear);
		break;

	case PiTokenEnumType::GarbageCollect:
		AppendNew(Operation::GarbageCollect);
		break;

	case PiTokenEnumType::ToRho:
		AppendNew(Operation::ToRho);
		break;
	}
}

KAI_END
