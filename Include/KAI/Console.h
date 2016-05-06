#pragma once

#include "KAI/ConsoleColor.h"
#include "KAI/Translator/Pi/Pi.h"
#include "KAI/Translator/Rho/Rho.h"

KAI_BEGIN

struct Coloriser;

class Console
{
	Tree tree;
	Registry *registry;
	Pointer<Executor> executor;
	Pointer<Compiler> compiler;
	Memory::IAllocator *alloc;
	Language language;

public:
	~Console();
	Console();
	Console(Memory::IAllocator *);
	Console(const std::vector<String> &args, Memory::IAllocator *);

	void Create(const std::vector<String> &);

	void SetLanguage(Language lang);
	void SetLanguage(int lang);
	int GetLanguage() const;

	String GetPrompt() const;
	String Process(const String&);
	Registry &GetRegistry() const { return *registry; }
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
	
	void Run();

protected:
	void CreateTree();
	void RegisterTypes();
	void ExposeTypesToTree(Object types);
};

KAI_END
