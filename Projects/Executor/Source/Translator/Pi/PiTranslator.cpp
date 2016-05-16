#include "KAI/ExecutorPCH.h"

using namespace boost;
using namespace std;

KAI_BEGIN

void PiTranslator::TranslateNode(AstNodePtr node)
{
	switch (node->GetType())
	{
		case PiAstNodeEnumType::Continuation:
		{
			PushNew();
			for (auto ch : node->GetChildren())
			{
				//cout << *ch << endl;
				if (ch->GetToken().type != PiTokenEnumType::NewLine)
					TranslateNode(ch);
			}
			Append(Pop());
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
		AppendNew(String(tok.Text()));
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
		AppendOp(Operation::Equiv);
		break;

	case PiTokenEnumType::Greater:
		AppendOp(Operation::Greater);
		break;

	case PiTokenEnumType::Rot:
		AppendOp(Operation::Rot);
		break;

	case PiTokenEnumType::PickN:
		AppendOp(Operation::Pick);
		break;

	case PiTokenEnumType::Clear:
		AppendOp(Operation::Clear);
		break;

	case PiTokenEnumType::GarbageCollect:
		AppendOp(Operation::GarbageCollect);
		break;

	case PiTokenEnumType::ToRho:
		AppendOp(Operation::ToRho);
		break;
	}
}

KAI_END
