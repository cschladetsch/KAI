#include <boost/lexical_cast.hpp>
#include "KAI/Language/Pi/PiTranslator.h"

using namespace boost;
using namespace std;

KAI_BEGIN

void PiTranslator::TranslateNode(AstNodePtr node)
{
	if (!node)
	{
		Fail("Empty input");
		return;
	}

	switch (node->GetType())
	{
		case PiAstNodeEnumType::Array:
		{
			// although we are getting an array,
			// pretend at first we are getting a Continuation
			// then use it's code as the data for the array
			PushNew();
			for (auto const &ch : node->GetChildren())
				TranslateNode(ch);
			Pointer<Continuation> c = Pop();
			Append(c->GetCode());
			break;
		}

		case PiAstNodeEnumType::Continuation:
		{
			PushNew();
			for (auto const &ch : node->GetChildren())
				TranslateNode(ch);
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

	case PiTokenEnumType::QuotedIdent:
	{
		auto label = Label(tok.Text().c_str());
		label.Quoted = true;
		AppendNew(label);
		break;
	}

	case PiTokenEnumType::True:
		AppendNew(true);
		break;

	case PiTokenEnumType::Assert:
		AppendOp(Operation::Assert);
		break;

	case PiTokenEnumType::False:
		AppendNew(false);
		break;

	case PiTokenEnumType::Bool:
		AppendNew(boost::lexical_cast<bool>(tok.Text()));
		break;

	case PiTokenEnumType::GetType:
		AppendOp(Operation::TypeOf);
		break;

	case PiTokenEnumType::Store:
		AppendOp(Operation::Store);
		break;

	case PiTokenEnumType::Lookup:
		AppendOp(Operation::Retreive);
		break;

	case PiTokenEnumType::Int:
		AppendNew(boost::lexical_cast<int>(tok.Text()));
		break;

	case PiTokenEnumType::Replace:
		AppendOp(Operation::Replace);
		break;

	case PiTokenEnumType::Suspend:
		AppendOp(Operation::Suspend);
		break;

	case PiTokenEnumType::Resume:
		AppendOp(Operation::Resume);
		break;

	case PiTokenEnumType::Drop:
		AppendOp(Operation::Drop);
		break;

	case PiTokenEnumType::Dup:
		AppendOp(Operation::Dup);
		break;

	case PiTokenEnumType::Assign:
		AppendOp(Operation::Assign);
		break;

	case PiTokenEnumType::Swap:
		AppendOp(Operation::Swap);
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

	case PiTokenEnumType::Divide:
		AppendOp(Operation::Divide);
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

	case PiTokenEnumType::Ident:
		AppendNew(Label(tok.Text()));
		break;

	case PiTokenEnumType::Pathname:
		AppendNew(Pathname(tok.Text()));
		break;

	case PiTokenEnumType::ToRho:
		AppendOp(Operation::ToRho);
		break;
	case PiTokenEnumType::None:
		break;

	case PiTokenEnumType::Size:
		AppendOp(Operation::Size);
		break;

	case PiTokenEnumType::ToArray:
		AppendOp(Operation::ToArray);
		break;

	case PiTokenEnumType::Depth:
		AppendOp(Operation::Depth);
		break;

	case PiTokenEnumType::Not:
		AppendOp(Operation::LogicalNot);
		break;

	case PiTokenEnumType::And:
		AppendOp(Operation::LogicalAnd);
		break;

	case PiTokenEnumType::Or:
		AppendOp(Operation::LogicalOr);
		break;

	case PiTokenEnumType::Xor:
		AppendOp(Operation::LogicalXor);
		break;

	default:
		KAI_TRACE_1(tok.type) << " Token not implemented: " << tok.ToString();
		// KAI_NOT_IMPLEMENTED_1(tok.ToString());
		break;
	}
}

KAI_END
