#include <KAI/Language/Hlsl/HlslLexer.h>

using namespace std;

KAI_BEGIN

void HlslLexer::AddKeyWords()
{
    keyWords["AppendStructuredBuffer"] = Enum::AppendStructuredBuffer;
    keyWords["asm"] = Enum::Asm;
    keyWords["asm_fragment"] = Enum::AsmFragment;
    keyWords["bool"] = Enum::Bool;
    keyWords["break"] = Enum::Break;
    keyWords["Buffer"] = Enum::Buffer;
    keyWords["ByteAddressI=Buffer"] = Enum::ByteAddressBuffer;
    keyWords["case"] = Enum::Case;
    keyWords["continue"] = Enum::Continue;
    keyWords["class"] = Enum::Class;
    keyWords["column"] = Enum::Column;
    keyWords["column_major"] = Enum::ColumnMajor;
    keyWords["Compile"] = Enum::Compile;
    keyWords["compile_fragment"] = Enum::CompileFragment;
    keyWords["CompileShader"] = Enum::CompileShader;
    keyWords["const"] = Enum::Const;
    keyWords["continue"] = Enum::Continue;
    keyWords["ComputeShader"] = Enum::ComputeShader;
    keyWords["ConsumeStructuredBuffered"] = Enum::ConsumeStructuredBuffered;
    keyWords["default"] = Enum::Default;
    keyWords["DepthStencilState"] = Enum::DepthStencilState;
    keyWords["DepthStencilView"] = Enum::DepthStencilView;
    keyWords["discard"] = Enum::Discard;
    keyWords["do"] = Enum::Do;
    keyWords["double"] = Enum::Double;
    keyWords["DomainShader"] = Enum::DomainShader;
    keyWords["dword"] = Enum::Dword;
    keyWords["else"] = Enum::Else;
    keyWords["export"] = Enum::Export;
    keyWords["extern"] = Enum::Extern;
    keyWords["false"] = Enum::False;
    keyWords["float"] = Enum::Float;
    keyWords["for"] = Enum::For;
    keyWords["fxgroup"] = Enum::Fxgroup;
    keyWords["GeometryShader"] = Enum::GeometryShader;
    keyWords["groupshared"] = Enum::GroupShared;
    keyWords["half"] = Enum::Half;
    keyWords["Hullshader"] = Enum::HullShader;
    keyWords["in"] = Enum::In;
    keyWords["inline"] = Enum::Inline;
    keyWords["inout"] = Enum::Inout;
    keyWords["InputPatch"] = Enum::InputPatch;
    keyWords["int"] = Enum::Int;
    keyWords["interface"] = Enum::Interface;
    keyWords["line"] = Enum::Line;
    keyWords["lineadj"] = Enum::Lineadj;
    keyWords["linear"] = Enum::Linear;
    keyWords["LineStream"] = Enum::LineStream;
    keyWords["matrix"] = Enum::Matrix;
    keyWords["min16float"] = Enum::Min16Float;
    keyWords["min10float"] = Enum::Min10Float;
    keyWords["min16int"] = Enum::Min16Int;
    keyWords["min12int"] = Enum::Min12Int;
    keyWords["namespace"] = Enum::Namespace;
    keyWords["nointerpolation"] = Enum::Nointerpolation;
    keyWords["noperspective"] = Enum::Noperspective;
    keyWords["NULL"] = Enum::Null;
    keyWords["out"] = Enum::Out;
    keyWords["OutputPatch"] = Enum::OutputPatch;
    keyWords["packoffset"] = Enum::PackOffset;
    keyWords["pass"] = Enum::Pass;
    keyWords["pixelfragment"] = Enum::PixelFragment;
    keyWords["PixelShader"] = Enum::PixelShader;
    keyWords["point"] = Enum::Point;
    keyWords["PointStream"] = Enum::PointStream;
    keyWords["precise"] = Enum::Precise;
    keyWords["RasterizerState"] = Enum::RasterizerState;
    keyWords["RenderTargetView"] = Enum::RenderTargetView;
    keyWords["return"] = Enum::Return;
    keyWords["register"] = Enum::Register;
    keyWords["row_major"] = Enum::RowMajor;
    keyWords["RWBuffer"] = Enum::RwBuffer;
    keyWords["WByteAddressBuffer"] = Enum::RwByteAddressBuffer;
    keyWords["RWStructuredBuffer"] = Enum::RwStructuredBuffer;
    keyWords["RWTexture1D"] = Enum::RwTexture1D;
    keyWords["RWTexture1DArray"] = Enum::RWTexture1DArray;
    keyWords["RWTexture2D"] = Enum::RwTexture2D;
    keyWords["RWTexture2DArray"] = Enum::RwTexture2DArray;
    keyWords["RWTexture3D"] = Enum::RwTexture3D;
    keyWords["sample"] = Enum::Sample;
    keyWords["sampler"] = Enum::Sampler;
    keyWords["SamplerState"] = Enum::SamplerState;
    keyWords["SamplerComparisonState"] = Enum::SamplerComparisonState;
    keyWords["shared"] = Enum::Shared;
    keyWords["snorm"] = Enum::Snorm;
    keyWords["stateblock"] = Enum::StateBlock;
    keyWords["stateblock_state"] = Enum::StateBlockState;
    keyWords["static"] = Enum::Static;
    keyWords["string"] = Enum::String;
    keyWords["struct"] = Enum::Struct;
    keyWords["switch"] = Enum::Switch;
    keyWords["structuredbuffer"] = Enum::StructuredBuffer;
    keyWords["tbuffer"] = Enum::TBuffer;
    keyWords["technique"] = Enum::Technique;
    keyWords["technique10"] = Enum::Technique10;
    keyWords["technique11"] = Enum::Technique11;
    keyWords["texture"] = Enum::Texture;
    keyWords["Texture1D"] = Enum::Texture1D;
    keyWords["Texture1DArray"] = Enum::Texture1DArray;
    keyWords["Texture2D"] = Enum::Texture2D;
    keyWords["Texture2DArray"] = Enum::Texture2DArray;
    keyWords["Texture2DMS"] = Enum::Texture2DMS;
    keyWords["Texture2DMSArray"] = Enum::Texture2DMSArray;
    keyWords["Texture3D"] = Enum::Texture3D;
    keyWords["TextureCube"] = Enum::TextureCube;
    keyWords["TextureCubeArray"] = Enum::TextureCubeArray;
    keyWords["true"] = Enum::True;
    keyWords["typedef"] = Enum::Typedef;
    keyWords["triangle"] = Enum::Triangle;
    keyWords["triangleadj"] = Enum::TriangleAdj;
    keyWords["TriangleStream"] = Enum::TriangleStream;
    keyWords["uint"] = Enum::Uint;
    keyWords["uniform"] = Enum::Uniform;
    keyWords["unorm"] = Enum::Unorm;
    keyWords["unsigned"] = Enum::Unsigned;
    keyWords["vector"] = Enum::Vector;
    keyWords["vertexfragment"] = Enum::VertexFragment;
    keyWords["VertexShader"] = Enum::VertexShader;
    keyWords["void"] = Enum::Void;
    keyWords["volatile"] = Enum::Volatile;
    keyWords["while"] = Enum::While;
}

bool HlslLexer::NextToken()
{
	char current = Current();
	if (current == 0)
		return false;

	if (isalpha(current))
		return LexPathname();

	if (isdigit(current))
		return Add(Enum::Int, Gather(isdigit));

	switch (current)
	{
	case '\'': return LexPathname();
	case ';': return Add(Enum::Semi);
	case '{': return Add(Enum::OpenBrace);
	case '}': return Add(Enum::CloseBrace);
	case '(': return Add(Enum::OpenParan);
	case ')': return Add(Enum::CloseParan);
	case ' ': return Add(Enum::Whitespace, Gather(IsSpaceChar));
	case ',': return Add(Enum::Comma);
	case '*': return Add(Enum::Mul);
	case '[': return Add(Enum::OpenSquareBracket);
	case ']': return Add(Enum::CloseSquareBracket);
	case '=': return AddIfNext('=', Enum::Equiv, Enum::Assign);
	case '!': return AddIfNext('=', Enum::NotEquiv, Enum::Not);
	case '&': return AddIfNext('&', Enum::And, Enum::BitAnd);
	case '|': return AddIfNext('|', Enum::Or, Enum::BitOr);
	case '<': return AddIfNext('=', Enum::LessEquiv, Enum::Less); 
	case '>': return AddIfNext('=', Enum::GreaterEquiv, Enum::Greater);
	case '"': return LexString(); // "
	case '\t': return Add(Enum::Tab);
	case '\n': return Add(Enum::NewLine);
	case '/':
		if (Peek() == '/')
		{
			Next();
			int start = offset;
			while (Next() != '\n')
				;

			Token comment(Enum::Comment, *this, lineNumber, Slice(start, offset));
			Add(comment);
			Next();
			return true;
		}
		return Add(Enum::Divide);

	case '-':
		if (Peek() == '-')
			return AddTwoCharOp(Enum::Decrement);
		if (Peek() == '=')
			return AddTwoCharOp(Enum::MinusAssign);
		return Add(Enum::Minus);

	case '.':

		return Add(Enum::Dot);

	case '+':
		if (Peek() == '+')
			return AddTwoCharOp(Enum::Increment);
		if (Peek() == '=')
			return AddTwoCharOp(Enum::PlusAssign);
		return Add(Enum::Plus);

	}

	LexError("Unrecognised %c");

	return false;
}

bool Contains(const char *allowed, char current);

// TODO: this is the same as PiLexer::PathnameOrKeyword(!)
bool HlslLexer::LexPathname()
{
	int start = offset;
	bool quoted = Current() == '\'';
	if (quoted)
		Next();

	bool rooted = Current() == '/';
	if (rooted)
		Next();

	bool prevIdent = false;
	do
	{
		Token result = LexAlpha();

		if (result.type != TokenEnumType::Ident)
		{
			// this is actually a keyword
			if (quoted || rooted)
			{
				return false;
			}

			// keywords cannot be part of a path
			if (prevIdent)
			{
				return false;
			}

			Add(result);
			return true;
		}

		prevIdent = true;

		auto isSeparator = Contains(Pathname::Literals::AllButQuote, Current());
		if (isSeparator)
		{
			Next();
			continue;
		}

		if (!isalpha(Current()))
		{
			break;
		}
	}
	while (true);

	//Add(Enum::Pathname, Slice(start, offset));

	return true;
}

void HlslLexer::Terminate()
{
	Add(Enum::None, 0);
}

KAI_END
