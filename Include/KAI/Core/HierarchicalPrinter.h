//#pragma once
//
//// prints a hierarchy of objects. each object must be insertable into the stream,
//// and have a method called GetChildren().
//template <class T, class Str = std::string, class Stream = std::stringstream>
//struct HierarchicalPrinter
//{
//	Str PrintTree(const T& root) const
//	{
//		Stream str;
//		PrintTree(str, 0, root);
//		str << std::ends;
//		return std::move(str.str());
//	}
//
//private:
//	void PrintTree(Stream &str, int level, const T& root) const
//	{
//		//str << KAI_NAMESPACE(ToString(*root)) << "\n";
//		str << root->ToString() << std::endl;
//		Str indent = Str(' ', 4 * level);
//		for (const auto  &ch : root->GetChildren())
//		{
//			str << indent;
//			PrintTree(str, level + 1, ch);
//		}
//	}
//};
