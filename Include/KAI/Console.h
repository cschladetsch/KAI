#pragma once

KAI_BEGIN

class Console
{
	Tree tree;
	Registry *registry;
	Pointer<Executor> executor;
	Pointer<Compiler> compiler;
	Memory::IAllocator *alloc;

public:
	~Console();
	Console();
	Console(Memory::IAllocator *);
	Console(const std::vector<String> &args, Memory::IAllocator *);

	void Create(const std::vector<String> &);

	String GetPrompt() const;
	String Process(const String&);
	Registry &GetRegistry() const { return *registry; }
	Tree &GetTree() { return tree; }
	Tree const &GetTree() const { return tree; }

	Object GetRoot() const { return tree.GetRoot(); }
	Pointer<Executor> GetExecutor() const { return executor; }
	Pointer<Compiler> GetCompiler() const { return compiler; }
	//Pointer<Continuation> Compile(const char *, Parser::Structure);
	Object ExecFile(const char *fileName);
	String Execute(Pointer<Continuation>);

	Pointer<Continuation> Execute(String const &text);
	void ExecuteFile(const char *);
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
