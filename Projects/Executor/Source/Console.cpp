#include "KAI/ExecutorPCH.h"
#include "KAI/ConsoleColor.h"

using namespace std;

KAI_BEGIN

Console::Console()
{
	alloc = make_shared<Memory::StandardAllocator>();
	Create();
}

Console::Console(shared_ptr<Memory::IAllocator> alloc)
{
	this->alloc = alloc;
	Create();
}

Console::~Console()
{
	alloc->DeAllocate(registry);
}

void Console::Create()
{
	try
	{
		registry = alloc->Allocate<Registry>(alloc);

		RegisterTypes();

		executor = registry->New<Executor>();
		compiler = registry->New<Compiler>();

		executor.SetManaged(false);
		compiler.SetManaged(false);

		CreateTree();
	}
	KAI_CATCH(exception, e)
	{
		cerr << "Console::Create::Exception '" << e.what() << "'" << ends;
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

void Console::SetLanguage(Language lang)
{
	SetLanguage(static_cast<int>(lang));
}

void Console::SetLanguage(int lang)
{
	language = static_cast<Language>(lang);
	compiler->SetLanguage(lang);
}

void Console::ControlC()
{
	executor->ClearContext();
}

int Console::GetLanguage() const
{
	return static_cast<int>(language);
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
	tree.AddSearchPath(Pathname("/Bin"));
	tree.AddSearchPath(Pathname("/Sys"));
	tree.AddSearchPath(Pathname("/Types"));

	executor->SetTree(&tree);
	registry->SetTree(tree);

	root.Set("Home", home);
	tree.SetScope(Pathname("/Home"));

	ExposeTypesToTree(types);
}

void Console::Execute(Pointer<Continuation> cont)
{
	KAI_TRY
	{
		if (!cont->HasScope())
			cont->SetScope(tree.GetRoot());

		executor->Continue(cont);
		//cout << WriteStack().c_str() << endl;
	}
	KAI_CATCH(Exception::Base, E)
	{
#ifdef KAI_DEBUG_TRACE
		KAI_TRACE_ERROR_1(E);
#else
		cerr << Color::Error << "Error: " << E.ToString() << endl;
#endif
	}
	KAI_CATCH(exception, E)
	{
#ifdef KAI_DEBUG_TRACE
		KAI_TRACE_ERROR_2("StdException: ", E.what());
#else
		cerr << Color::Error << "StdError: " << E.what() << endl;
#endif
	}
	KAI_CATCH_ALL()
	{
#ifdef KAI_DEBUG_TRACE
		KAI_TRACE_ERROR_1("UnknownException");
#else
		cerr << Color::Error << "UnknownException" << endl;
#endif
	}
}

void Console::Execute(String const &text, Structure st)
{
	auto cont = compiler->Translate(text.c_str(), st);
	if (!cont)
		return;
	executor->Continue(cont);
}

String Console::Process(const String& text)
{
	StringStream result;
	KAI_TRY
	{
		cout << Color::Error;
		auto cont = compiler->Translate(text.c_str());
		if (cont)
		{
			cont->SetScope(tree.GetScope());
			cout << Color::Trace;
			Execute(cont);
		}

		return "";
	}
	KAI_CATCH(Exception::Base, E)
	{
		result << "Exception: " << E.ToString() << "\n";
	}
	KAI_CATCH(exception, E)
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
		<< Color::LanguageName << ToString((Language)compiler->GetLanguage())
		<< Color::Pathname << GetFullname(GetTree().GetScope()).ToString().c_str() << Color::Input << "> ";

	return prompt.ToString();
}

String Console::WriteStack() const
{
	Value<const Stack> data = executor->GetDataStack();
	Stack::const_iterator A = data->Begin(), B = data->End();
	StringStream result;
	for (int N = data->Size() - 1; A != B; ++A, --N)
	{
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

int Console::Run()
{
	for (;;)
	{
		KAI_TRY
		{
			for (;;)
			{
				cout << Color::Prompt << GetPrompt().c_str() << Color::Input;
				string text;
				getline(cin, text);
				cout << Color::Trace << Process(text.c_str()).c_str();

				cout << executor->PrintStack() << endl;

				if (_end)
					return _endCode;
			}
		}
		KAI_CATCH(Exception::Base, E)
		{
			KAI_TRACE_ERROR_1(E);
		}
		KAI_CATCH(exception, E)
		{
			KAI_TRACE_ERROR_1(E.what());
		}
		KAI_CATCH_ALL()
		{
			KAI_TRACE_ERROR() << " something went wrong";
		}
	}
}

void Console::RegisterTypes()
{
	// built-ins
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
	
	// TODO: remove less-than comparable trait for hash maps: 
	//HashMap::Register(*registry, "HashMap");

#ifdef KAI_UNIT_TESTS
	registry->AddClass<Test::IOutput *>("TestOutputBase");
	Test::Summary::Register(*registry);
	Test::Module::Register(*registry, "TestModule");
	Test::BasicOutput::Register(*registry);
	Test::XmlOutput::Register(*registry);
#endif
}

Pointer<Continuation> Console::Compile(const char *text, Structure st)
{
	return compiler->Translate(text, st);
}

void Console::Register(Registry &)
{
}

void Console::ExecuteFile(const char *fileName)
{
	auto c = compiler->CompileFile(fileName, Structure::Program);
	executor->Continue(c);
}

KAI_END

//EOF
