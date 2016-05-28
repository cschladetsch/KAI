#pragma once

#include <KAI/Core/Config/Base.h>
#include <KAI/Language/Common/TranslatorBase.h>
#include "KAI/Core/BuiltinTypes/Array.h"

#include "Tau/TauParser.h"

TAU_BEGIN

	class TauTranslator : public TranslatorBase<TauParser>
	{
	public:
		typedef TranslatorBase<TauParser> Parent;
		typedef typename Parent::Parser Parser;
		typedef typename Parent::TokenNode TokenNode;
		typedef typename Parent::AstNode AstNode;
		typedef typename Parent::TokenEnum TokenEnum;
		typedef typename Parent::AstEnum AstNodeEnum;
		typedef typename Parent::AstNodePtr AstNodePtr;

		TauTranslator(Registry &r) : Parent(r) { }

	protected:
		virtual void TranslateNode(AstNodePtr node) override;

	private:
		void TranslateToken(AstNodePtr node);
		void TranslatePreduict(AstNodePtr node);
		void TranslateBlock(AstNodePtr node);
		void TranslateIndex(AstNodePtr node);
	};

TAU_END
