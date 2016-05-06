#include "KAI/ExecutorPCH.h"
#include "KAI/Translator/Pi/PiTranslator.h"

#include <boost/lexical_cast.hpp>

KAI_BEGIN

void PiTranslator::TranslateNode(AstNodePtr node)
{
//	std::cout << ToString() << std::endl;

	switch (node->GetType())
	{
		case PiAstNodeEnumType::Continuation:
		{
			PushNew();
			for (auto const &ch : node->GetChildren())
			{
				TranslateNode(ch);
			}
			stack.push_back(Pop());
			break;
		}

		default:
		{
			if (node->GetToken().type != PiTokenEnumType::NewLine)
				AppendTokenised(node->GetToken());
			break;
		}
	}
}

void PiTranslator::AppendTokenised(const TokenNode& tok)
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
