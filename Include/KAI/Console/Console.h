#pragma once

#include <memory>
#include <KAI/Core/Reflected.h>
#include <KAI/Core/Tree.h>
#include <KAI/Core/Registry.h>
#include <KAI/Core/Executor.h>
#include <KAI/Core/Pointer.h>
#include <KAI/Core/Compiler.h>
#include <KAI/Core/Memory/IAllocator.h>
#include <KAI/Core/BuiltinTypes/String.h>
#include "KAI/Core/Config/Base.h"
#include "KAI/Core/Type/Traits.h"
#include <KAI/Core/Object.h>
#include <KAI/Core/Type/Number.h>
#include <KAI/Core/Type/TraitMacros.h>
#include <KAI/Core/Type/Properties.h>

#include <KAI/Executor/Continuation.h>

#include <KAI/Language/Common/Structure.h>
#include <KAI/Language/Common/Language.h>
#include "KAI/Language/Pi/Pi.h"
#include "KAI/Language/Rho/Rho.h"

#include "KAI/Console/ConsoleColor.h"

KAI_BEGIN

struct Coloriser;

class Console : public Reflected
{
	Tree tree;
	Registry *registry;
	Pointer<Executor> executor;
	Pointer<Compiler> compiler;
	std::shared_ptr<Memory::IAllocator> alloc;
	Language language;

public:
	Console();
	Console(std::shared_ptr<Memory::IAllocator>);
	~Console();

	void SetLanguage(Language lang);
	void SetLanguage(int lang);
	Language GetLanguage() const;

	String GetPrompt() const;
	String Process(const String&);
	Registry &GetRegistry() const { return *registry; }
	Tree &GetTree() { return tree; }
	Tree const &GetTree() const { return tree; }

	Object GetRoot() const { return tree.GetRoot(); }
	Pointer<Executor> GetExecutor() const { return executor; }
	Pointer<Compiler> GetCompiler() const { return compiler; }

	Pointer<Continuation> Compile(const char *, Structure);
	void Execute(const String &text, Structure st = (Structure) Structure::Statement);
	void ExecuteFile(const char *);
	void Execute(Pointer<Continuation> cont);

	String WriteStack() const;
	void ControlC();
	static void Register(Registry &);
	
	int Run();

protected:
	void Create();
	void CreateTree();
	void RegisterTypes();
	void ExposeTypesToTree(Object types);

private:
	bool _end = false;
	int _endCode = 0;
};

KAI_TYPE_TRAITS(Console, Number::Console, Properties::Reflected);

KAI_END
`
