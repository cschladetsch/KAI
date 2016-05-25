#include "Main.h"
#include <string>
#include <vector>
#include <map>

struct Gamma
{

	typedef std::string String;
	typedef std::vector<String> Text;

	Text _text;

	/// the current input state
	enum class EState
	{
		None,
		Prefix,
		Command,
		Arguments,
		Insert,
	};

	enum class EScope
	{
		None,
		UntilWhitespace,
		UntilNotWhitespace,
		UntilAlpha,
		UntilNotAlpha,
		UntilNumber,
		UntilNotNumber,
		UntilPunc,
		UntilNotPunc,
	};

	/// the action we're going to do
	enum class EAction
	{
		None,
		Insert,
		Change,
		Delete,
		Copy,
		Paste,
		Move,
		FindChar,
		AppendAfterChar,
		AppendAtChar,
		Repeat,
		Undo,
		Redo,
		Mark,
		GotoMark,
	};

	/// where we are going to do our action
	enum class EArgument
	{
		None,
		Any,
		Char,
		UntilWhitespace,
		UntilNonAlpha,
		UntilAlpha,
	};

	struct IHandler
	{
		virtual bool Process(char c) = 0;
	};

	struct HandlerCommon : IHandler
	{
		Gamma &_gamma;

		HandlerCommon(Gamma &g) : _gamma(g)
		{
		}

		virtual bool Process(char c) override
		{
		}
	};

	struct NoneHandler : HandlerCommon
	{
		using HandlerCommon::_gamma;

		int Count = 0;
		typedef std::map<char, std::function<int(int, HandlerCommon)>> Actions;

		Actions _actions;

		NoneHandler(Gamma &g) : HandlerCommon(g)
		{
//			_actions['i'] = [&Count]() { Insert }
//			_actions['y'] = []() { make_shared<ActionHandler>()->Copy(Count); };
//			_actions['p'] = []() { make_shared<ActionHandler>()->Past(Count); };
//			_actions['s'] = []() { make_shared<ActionHandler>()->Substitue(Count); };
		}

		virtual bool Process(char c) override
		{
			if (isdigit(c))
			{
				Count *= 10;
				Count += c - '0';
				return true;
			}

			if (_actions.find(c) != _actions.end())
			{
//				_gamma
			}

			return false;
		}
	};

	enum class ECharType
	{
		None,
		Escape,
		Number,
		Alpha,
		Punc,
		Operator,
		Any
	};

	enum class EIntent
	{
		None,
		Escape,
		Insert,
	};

	std::map<char, ECharType> _types;

	EIntent Intet() const
	{
		return EIntent::None;
	}

	struct Location
	{
		int line = 0;
		int col = 0;
		Gamma &gamma;

		Location(Gamma &g, int l, int c)
			: gamma(g), line(l), col(c)
		{
		}

		void Insert(String const &s)
		{
			gamma.Insert(line, col, s);
		}
	};


//	vector<Command> _commands;

	void Insert(int line, int col, const String &s)
	{
//		_commands.Add([]() { InsertText(line, col, s); });
//		_undo.Add([]() { Delete(line, col, s); });
	}


	void InsertText()
	{
	}

	template<class Action>
	struct Repeater
	{
		Action _action;

		Repeater(int count, Gamma &c)
		{
			_action.Init(c);
			for (int n = 0;
			     n < count;
			     ++n)
			{
				_action.Perform(c);
			}
			_action.Complete(c);
		}
	};

	struct ActionHandler : IHandler
	{
		virtual bool Process(char c) override
		{
			return false;
		}

		void InsertText(int count)
		{
		}

	};

	struct CommandHandler : IHandler
	{
		virtual bool Process(char c) override
		{
			return false;
		}
	};

	struct Delta
	{
		String Before;
		String After;
	};

	struct Command
	{
		EAction _action;
		EArgument _argument;
	};

	typedef std::vector<Delta> Deltas;

	Deltas _deltas;
	EState _state;
	std::shared_ptr<IHandler> _handler;

	Gamma()
	{
		Escape();
	}

	Gamma(const String &contents, int line, int col)
		: _text(Split(contents, '\n"')
	{
	}

	std:vector<String> Split(String const &inout)
	{
		return vector<String>();
	}

	void Escape()
	{
		_state = EState::None;
		_handler = std::make_shared<NoneHandler>();
	}

	void Input(char c)
	{
		_handler->Process(c);
	}

	bool Process(String &existing, String const &input)
	{
		return false;
	}
};

