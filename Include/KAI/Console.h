

#ifdef KAI_HAVE_PRAGMA_ONCE
#	pragma once
#endif

#ifndef KAI_CONSOLE_H
#	define KAI_CONSOLE_H

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
	Console(Memory::IAllocator *);
	Console(const nstd::vector<String> &args, Memory::IAllocator *);

	void Create(const nstd::vector<String> &);

	String GetPrompt() const;
	String Process(const String&);
	Registry &GetRegistry() const { return *registry; }
	Tree &GetTree() { return tree; }
	Tree const &GetTree() const { return tree; }

	Object GetRoot() const { return tree.GetRoot(); }
	Pointer<Executor> GetExecutor() const { return executor; }
	Pointer<Compiler> GetCompiler() const { return compiler; }
	Pointer<Continuation> Compile(const char *);
	String Execute(Pointer<Continuation>);

	Object Execute(String const &text);
	void ExecuteFile(const char *);
	String WriteStack() const;
	void ControlC();
	static void Register(Registry &);

protected:
	void CreateTree();
	void RegisterTypes();
	void ExposeTypesToTree(Object types);
};

KAI_END

#endif // KAI_CONSOLE_H

//EOF

