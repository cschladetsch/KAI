#include "KAI/KAI.h"
#include "KAI/DotGraph.h"
#include "KAI/Continuation.h"
#include <fstream>

using namespace std;
using namespace kai;

KAI_BEGIN

DotGraph::DotGraph()
{
}

DotGraph::DotGraph(Object root, String const &filename)
{
	Generate(root, filename);
}

void DotGraph::Generate(Object root, String const &filename)
{
	this->Clear();
	WriteHeader();
	*this << root;
	WriteFooter();
	WriteToFile(filename.c_str());
}

void DotGraph::WriteHeader()
{
	*this << "digraph G\n{\n";
}

void DotGraph::WriteFooter()
{
	*this << "}\n";
}

void DotGraph::ExcludeLabel(Label const &label)
{
	excluded_names.insert(label);
}

String DotGraph::GetText()
{
	return ToString();
}

DotGraph &operator<<(DotGraph &graph, Object const &object)
{
	bool excluded = !object || graph.IsExcluded(object);
	if (excluded)
		return graph;

	int object_id = object.GetHandle().GetValue();
	StringStream label;
	label << object.GetLabel().ToString() << "[" << object.GetClass()->GetName() << "]";
	if (excluded)
		label << " (excl.)";

	graph << object_id << " [shape=box label=\"" << label << "\"";
	graph << " color=";
	switch (object.GetColor())
	{
	case ObjectColor::White: graph << "red"; break;
	case ObjectColor::Grey: graph << "blue"; break;
	case ObjectColor::Black: graph << "green"; break;
	}

	graph << "]\n";
	if (excluded)
		return graph;

	if (object.IsType<Continuation>())
	{
		return graph;
	}

	Object::ObjectList props, children, contained;
	object.GetPropertyObjects(props);
	object.GetContainedObjects(contained);
	object.GetChildObjects(children);

	for (auto const &child : children)
	{
		graph << object_id << " -> " << child.GetHandle().GetValue() << "[weight=10]\n";
		graph << child;
	}

	for (auto const &prop : props)
	{
		//graph << object_id << "[label=\"" << prop.GetLabel() << "\"]\n";
		graph << object_id << " -> " << prop.GetHandle().GetValue() << "[style=dashed]\n";
		graph << prop;
	}

	for (auto const &cont : contained)
	{
		graph << object_id << " -> " << cont.GetHandle().GetValue() << "[style=dotted]\n";
		graph << cont;
	}

	return graph;
}

bool DotGraph::IsExcluded(Object const&object) const
{
	if (!object)
		return true;
	return excluded_types.find(object.GetTypeNumber()) != excluded_types.end() || excluded_names.find(object.GetLabel()) != excluded_names.end();
}

void DotGraph::WriteToFile(const char *filename)
{
	String text = GetText();
	std::fstream(filename, std::ios::out).write(text.c_str(), text.size());
}

KAI_END