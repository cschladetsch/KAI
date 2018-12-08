#pragma once

#include <KAI/Language/Common/ParserCommon.h>
#include <KAI/Language/Hlsl/HlslAstNode.h>
#include <KAI/Language/Hlsl/HlslLexer.h>

KAI_BEGIN

// parser specific to the in-fix Hlsl language
class HlslParser : public ParserCommon<HlslLexer, HlslAstNodeEnumType>
{
public:
	typedef ParserCommon<HlslLexer, HlslAstNodeEnumType> Parent;
	using typename Parent::TokenEnum;
	using typename Parent::TokenNode;
	using typename Parent::Lexer;
	using typename Parent::AstNode;
	using typename Parent::AstNodePtr;

	typedef HlslAstNodeEnumType NodeType; 
	typedef HlslTokenEnumType TokenType;

	HlslParser(Registry &r) : Parent(r) { }

	virtual bool Process(std::shared_ptr<Lexer> lex, Structure st) override;

protected:
	void Process(Structure);

private:
	bool Run(Structure st);
	bool Program();
	bool Statement(AstNodePtr);
	bool Expression();
	bool Logical();
	bool Relational();
	bool Additive();
	bool Term();
	bool Factor();
	void ConsumeNewLines();
	void Block(AstNodePtr block);
	bool ParsePathname();
	bool ParsePathname(AstNodePtr path);
	bool ParseFactorIdent();
	void ParseGetMember();
	void ParseMethodCall();
	void Function(AstNodePtr);
	void AddBlock(AstNodePtr fun);
	void IfCondition(AstNodePtr);
	void ParseIndexOp();
	void Assignment(AstNodePtr);
	void For(AstNodePtr);
	void While(AstNodePtr);

	bool CreateError(const char *);
};

KAI_END
