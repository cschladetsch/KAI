#include "KAI/KAI.h"
#include "KAI/Operation.h"
#include "KAI/BuiltinTypes/All.h"
#include "KAI/Continuation.h"
#include "KAI/PropertyBase.h"
#include "KAI/Value.h"
#include "KAI/Compiler.h"
#include "KAI/Reflected.h"
#include "KAI/Executor.h"
#include "KAI/BuiltinTypes/All.h"
#include "KAI/Pathname.h"
#include "KAI/Tree.h"
#include "KAI/Test/Base.h"
#include "KAI/Bin/Base.h"
#include "KAI/Console.h"
#include "KAI/BuiltinTypes/List.h"

#include <iostream>

KAI_BEGIN

bool ExecuteFile(const char *filename, Pointer<Executor> executor, Pointer<Compiler> compiler, Object scope);
void AddExamples(Object const &Q);

void Console::ControlC()
{
	executor->ClearContext();
}

Console::Console(Memory::IAllocator *alloc)
{
	registry = alloc->Allocate<Registry>(alloc);
	nstd::vector<String> args;
	Create(args);
	this->alloc = alloc;
}

Console::Console(const nstd::vector<String> &args, Memory::IAllocator *alloc)
{
	registry = alloc->Allocate<Registry>(alloc);
	Create(args);
	this->alloc = alloc;
}

Console::~Console()
{
	alloc->DeAllocate(registry);
}

void Console::Create(const nstd::vector<String> &args)
{
	try
	{
		RegisterTypes();

		executor = registry->New<Executor>();
		compiler = registry->New<Compiler>();

		executor.SetManaged(false);
		compiler.SetManaged(false);

		CreateTree();

		//if (args.size() == 1)
		//	KAI_NAMESPACE_NAME::ExecuteFile(args[0].c_str(), executor, compiler, tree.GetScope());
	}
	catch (Exception::Base &E)
	{
		std::cerr << "Console::Create::Exception '" << E.ToString().c_str() << "'" << std::ends;
	}
}

void Console::ExposeTypesToTree(Object types)
{
	for (int N = 0; N < Type::Number::Last; ++N)
	{
		const ClassBase *K = registry->GetClass(N);
		if (K == 0)
			continue;
		types.Set(K->GetName(), registry->New(K));
	}
}

void Console::CreateTree()
{
	Object root = registry->New<void>();
	Object types = registry->New<void>();
	Object sys = registry->New<void>();
	Object bin = registry->New<void>();
	Object home = registry->New<void>();

	//types.SetSwitch(IObject::NoRecurse, true);
	//sys.SetSwitch(IObject::NoRecurse, true);
	//root.SetSwitch(IObject::NoRecurse, true);
	//bin.SetSwitch(IObject::NoRecurse, true);

	types.SetSwitch(IObject::Managed, false);
	sys.SetSwitch(IObject::Managed, false);
	root.SetSwitch(IObject::Managed, false);
	bin.SetSwitch(IObject::Managed, false);

	home.SetManaged(false);

	tree.SetRoot(root);
	root.Set("Types", types);
	root.Set("Sys", sys);
	root.Set("Bin", bin);
	root.Set("Home", home);

	Set(root, Pathname("/Compiler"), compiler);
	Set(root, Pathname("/Executor"), executor);

	Bin::AddFunctions(bin);
	//bin.Set("RunTests", compiler->Compile(registry, "TestOutput new Tests 'Run ->"));
	//bin.Set("LoadContinuation", compiler->Compile(registry, "ReadFile () /Sys/Compiler 'Compile ->"));
	//bin.Set("RunFile", compiler->Compile(*registry, "LoadContinuation & !"));

	tree.AddSearchPath(Pathname("/Bin"));
	tree.AddSearchPath(Pathname("/Sys"));
	tree.AddSearchPath(Pathname("/Types"));

	executor->SetTree(&tree);
	registry->SetTree(tree);

	root.Set("Home", home);
	tree.SetScope(Pathname("/Home"));

	ExposeTypesToTree(types);
}

void Console::ExecuteFile(const char *filename)
{
	KAI_NAMESPACE_NAME::ExecuteFile(filename, executor, compiler, tree.GetScope());
}

String Console::Execute(Pointer<Continuation> cont)
{
	StringStream result;
	KAI_TRY
	{
		executor->Continue(cont);
		return WriteStack();
	}
	KAI_CATCH(Exception::Base, E)
	{
		result << "Exception: " << E.ToString() << "\n";
	}
	KAI_CATCH(std::exception, E)
	{
		result << "StdException: " << E.what() << "\n";
	}
	KAI_CATCH_ALL()
	{
		result << "UnknownException: " << "\n";
	}
	return result.ToString();
}

Object Console::Execute(String const &text)
{
	StringStream result;
	KAI_TRY
	{
		Pointer<Continuation> continuation = compiler->Compile(*registry, text.c_str());
		continuation->SetScope(tree.GetScope());
		executor->Continue(continuation);
	}
	KAI_CATCH(Exception::Base, E)
	{
		result << "Exception: " << E.ToString() << "\n";
	}
	KAI_CATCH(std::exception, E)
	{
		result << "StdException: " << E.what() << "\n";
	}
	KAI_CATCH_ALL()
	{
		result << "UnknownException: " << "\n";
	}
	return executor->GetDataStack();
}


String Console::Process(const String& text)
{
	StringStream result;
	KAI_TRY
	{
		Pointer<Continuation> continuation = compiler->Compile(*registry, text.c_str());
		continuation->SetScope(tree.GetScope());
		return Execute(continuation);
	}
	KAI_CATCH(Exception::Base, E)
	{
		result << "Exception: " << E.ToString() << "\n";
	}
	KAI_CATCH(std::exception, E)
	{
		result << "StdException: " << E.what() << "\n";
	}
	KAI_CATCH_ALL()
	{
		result << "UnknownException: " << "\n";
	}
	return result.ToString();
}

String Console::GetPrompt() const
{
	StringStream prompt;
	prompt
		<< int(GetRegistry().GetInstances().size()) << "; " 
		<< GetFullname(GetTree().GetScope()).ToString().c_str() << "> ";
	return prompt.ToString();
}

String Console::WriteStack() const
{
	Value<const Stack> data = executor->GetDataStack();
	Stack::const_iterator A = data->Begin(), B = data->End();
	StringStream result;
	for (int N = data->Size() - 1; A != B; ++A, --N)
	{
		//S << "[" << N << "]:" << A->GetHandle().GetValue() << ":" << A->GetParentHandle().GetValue() << "= " << *A << "\n" << Ends;
		result << "[" << N << "] ";
		bool is_string = A->GetTypeNumber() == Type::Number::String;
		if (is_string)
			result << "\"";
		result << *A;
		if (is_string)
			result << "\"";
		result << "\n";
	}

	return result.ToString();
}

void Console::RegisterTypes()
{
	// builtins
	registry->AddClass<const ClassBase *>("Class");		// TODO: add methods
	registry->AddClass<void>("Void");
	registry->AddClass<bool>("Bool");
	registry->AddClass<int>("Int");
	registry->AddClass<float>("Float");
	registry->AddClass<Vector3>("Vector3");
	registry->AddClass<Vector4>("Vector4");

	// system types
	ObjectSet::Register(*registry);
	String::Register(*registry);
	Object::Register(*registry);
	Handle::Register(*registry);
	Stack::Register(*registry);
	Continuation::Register(*registry);
	Label::Register(*registry);
	Operation::Register(*registry);
	Compiler::Register(*registry);
	Executor::Register(*registry);
	Pathname::Register(*registry);
	BasePointerBase::Register(*registry);
	Pair::Register(*registry);
	FunctionBase::Register(*registry);
	BasePointer<MethodBase>::Register(*registry);
	BasePointer<PropertyBase>::Register(*registry);
	BinaryStream::Register(*registry);
	StringStream::Register(*registry);
	Array::Register(*registry);
	List::Register(*registry);
	Map::Register(*registry, "Map");
	
	// CJS TODO: remove less-than comparable trait for hash maps: 
	//HashMap::Register(*registry, "HashMap");

#ifdef KAI_UNIT_TESTS
	registry->AddClass<Test::IOutput *>("TestOutputBase");
	Test::Summary::Register(*registry);
	Test::Module::Register(*registry, "TestModule");
	Test::BasicOutput::Register(*registry);
	Test::XmlOutput::Register(*registry);
#endif
}

Pointer<Continuation> Console::Compile(const char *text, Parser::Structure st)
{
	return compiler->Compile(*registry, text, st);
}

void Console::Register(Registry &)
{
}

KAI_END

//EOF
