#pragma once

#include <KAI/Executor/Compiler.h>
#include <KAI/Executor/Executor.h>
#include <KAI/Core/Tree.h>
#include <KAI/Language.h>
#include <KAI/Console/ConsoleColor.h>

KAI_BEGIN

struct Coloriser;

// This is a language-independant REPL console
class Console : public Reflected
{
	Tree tree;
	Registry *_reg;
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
	Registry &GetRegistry() const { return *_reg; }
	Tree &GetTree() { return tree; }
	Tree const &GetTree() const { return tree; }

	Object GetRoot() const { return tree.GetRoot(); }

	Pointer<Executor> GetExecutor() const { return executor; }
	Pointer<Compiler> GetCompiler() const { return compiler; }

	Pointer<Continuation> Compile(const char *, Structure);
	void Execute(const String &text, Structure st = Structure::Statement);
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

