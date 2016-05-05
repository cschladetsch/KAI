#include "KAI/ExecutorPCH.h"

#ifdef WIN32
#	define WIN32_LEAN_AND_MEAN
#	include <windows.h>

struct C
{
	enum What
	{
		Prompt,
		Input,
		Trace,
		Error,
		Warning,
		Last
	};
	static WORD colors[Last];
	static HANDLE hstdout;
	static CONSOLE_SCREEN_BUFFER_INFO orig;

	C()
	{
		colors[Prompt] = FOREGROUND_GREEN;
		colors[Input] = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;
		colors[Trace] = FOREGROUND_BLUE | FOREGROUND_GREEN;
		colors[Error] = FOREGROUND_RED;
		colors[Warning] = FOREGROUND_GREEN | FOREGROUND_RED;

		hstdout = GetStdHandle(STD_OUTPUT_HANDLE);

		// Remember how things were when we started
		GetConsoleScreenBufferInfo(hstdout, &orig);
	}

	~C()
	{
		SetConsoleTextAttribute(hstdout, orig.wAttributes);
	}

	static void SetColor(What c)
	{
		SetConsoleTextAttribute(hstdout, colors[c]);
	}

	void operator()(What c) const { SetColor(c); }
};
#endif

WORD C::colors[C::Last];
HANDLE C::hstdout;
CONSOLE_SCREEN_BUFFER_INFO C::orig;

struct color
{
	C::What col;
	color(C::What c) : col(c) { }

	friend std::ostream& operator<<(std::ostream& out, color const &c)
	{
		C::SetColor(c.col);
		return out;
	}
};

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
	std::vector<String> args;
	Create(args);
	static C c;
	this->alloc = alloc;
	SetLanguage(Language::Rho);
}

Console::Console(const std::vector<String> &args, Memory::IAllocator *alloc)
{
	registry = alloc->Allocate<Registry>(alloc);
	Create(args);
	this->alloc = alloc;
	SetLanguage(Language::Rho);
}

Console::Console()
{
	registry = alloc->Allocate<Registry>();
	Create(std::vector<String>());
	SetLanguage(Language::Rho);
}

Console::~Console()
{
	alloc->DeAllocate(registry);
}

void Console::Create(const std::vector<String> &args)
{
	KAI_UNUSED(args);
	try
	{
		RegisterTypes();

		executor = registry->New<Executor>();
		compiler = registry->New<Compiler>();

		executor.SetManaged(false);
		compiler.SetManaged(false);

		CreateTree();
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

void Console::SetLanguage(Language lang)
{
	SetLanguage(static_cast<int>(lang));
}

void Console::SetLanguage(int lang)
{
	language = static_cast<Language>(lang);
	compiler->SetLanguage(lang);
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

//void Console::ExecuteFile(const char *filename)
//{
//	KAI_NAMESPACE(ExecuteFile)(filename, executor, compiler, tree.GetScope());
//}

void Console::Execute(Pointer<Continuation> cont)
{
	KAI_TRY
	{
		if (!cont->HasScope())
			cont->SetScope(tree.GetRoot());

		executor->Continue(cont);
		//return WriteStack();
		//return WriteStack();
	}
	KAI_CATCH(Exception::Base, E)
	{
		KAI_TRACE_ERROR_1(E);
		std::cerr << color(C::Error) << "Error: " << E.ToString() << std::endl;
	}
	KAI_CATCH(std::exception, E)
	{
		KAI_TRACE_ERROR_2("StdException: ", E.what());
		std::cerr << color(C::Error) << "Error: " << E.what() << std::endl;
	}
	KAI_CATCH_ALL()
	{
		KAI_TRACE_ERROR_1("UnknownException");
		std::cerr << color(C::Error) << "Error" << std::endl;
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
		std::cout << color(C::Error);
		Pointer<Continuation> cont = compiler->Translate(text.c_str());
		if (cont)
		{
			cont->SetScope(tree.GetScope());
			std::cout << color(C::Trace);
			Execute(cont);
			return executor->PrintStack();
		}

		return "";
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
		<< ToString((Language)compiler->GetLanguage()) << ": " << int(GetRegistry().GetInstances().size()) << "; " 
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

void Console::Run()
{
	for (;;)
	{
		std::cout << color(C::Prompt) << GetPrompt().c_str() << color(C::Input);
		std::string text;
		std::getline(std::cin, text);
		std::cout << color(C::Trace) << Process(text.c_str()).c_str();
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
