
#ifdef KAI_HAVE_PRAGMA_ONCE
#	pragma once
#endif

#ifndef KAI_CLASS_BUILDER_H
#	define KAI_CLASS_BUILDER_H

KAI_BEGIN

void RegisterClass(Registry &, ClassBase const &klass, Object const &root, Pathname const &path);
/*
{
	r.AddClass(klass);
	if (root.Exists() && !path.Empty())
		Set(root, path, registry->NewFromClass(klass));
}
*/

template <class T>
class ClassBuilder
{
public:
	typedef Type::Traits<T> Traits;
	Registry *registry;
	Class<T> *klass;
	Pathname path;
	Object root;

	struct MethodsCollector
	{
		struct PropertiesCollector
		{
			ClassBuilder<T> *builder;
			PropertiesCollector &operator=(PropertiesCollector &);

			template <class Property>
			PropertiesCollector &operator()(Label const &N, Property P, String const &D = "", CreateParams::Params create_params = CreateParams::Default)
			{
				PropertyBase *Q = MakeProperty<T>(P, N, create_params);
				if (!D.empty())
					Q->Description = D;
				builder->klass->AddProperty(N, Q);
				return *this;
			}
		};

		ClassBuilder<T> *builder;
		PropertiesCollector Properties;
		MethodsCollector &operator=(MethodsCollector&);

		template <class Method>
		MethodsCollector &operator()(Label const &name, Method method, String const &D = "")
		{
			MethodBase *M = MakeMethod(method, name);
			if (!D.empty())
				M->Description = D;
			builder->klass->AddMethod(name, M);
			return *this;
		}
	};
	MethodsCollector Methods;

	ClassBuilder(Registry &R, String const &N) : registry(&R), klass(new Class<T>(N.c_str()))
	{ 
		Methods.builder = this;
		Methods.Properties.builder = this;
	}
	ClassBuilder(Registry &R, const Label &N, Object const &Q, const Pathname &P) : registry(&R), root(Q), path(P), klass(new Class<T>(N))
	{ 
		Methods.builder = this;
		Methods.Properties.builder = this;
	}

	~ClassBuilder()
	{
		RegisterClass(*registry, *klass, root, path);
	}
};

KAI_END

#endif 
