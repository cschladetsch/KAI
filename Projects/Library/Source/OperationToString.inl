

const char *Operation::ToString(int value)
{
	switch (value)
	{
		#define CASE(N) case N: return #N;
		CASE(Return);
		CASE(None);
		CASE(True);
		CASE(False);
		CASE(Less);
		CASE(Equiv); 
		CASE(Greater); 
		CASE(LessOrEquiv); 
		CASE(GreaterOrEquiv);
		CASE(Suspend);
		CASE(Replace);
		CASE(Resume);
		CASE(SetManaged);
		CASE(CppFunctionCall);
		CASE(CppMethodCall);
		CASE(Plus);
		CASE(Minus); 
		CASE(Multiply); 
		CASE(Divide);
		CASE(Modulo);
		CASE(LevelStack);
		CASE(TypeOf);
		CASE(Store);
		CASE(Retreive); 
		CASE(Remove);
		CASE(New);
		CASE(ChangeScope);
		CASE(GetChildren);
		CASE(Drop);
		CASE(DropN); 
		CASE(Swap);
		CASE(Dup);
		CASE(Rot);
		CASE(RotN); 
		CASE(Pick);
		CASE(Clear); 
		CASE(Depth);
		CASE(Over);
		CASE(ToArray); 
		CASE(ToList);
		CASE(Expand);
		CASE(Name);
		CASE(Fullname);
		CASE(TraceAll);
		CASE(Trace);
		CASE(This);
		CASE(Ref);
		CASE(Delete);
		CASE(Detach);
		CASE(Exists);
		CASE(Pin);
		CASE(Unpin);
		CASE(ContinuationBegin); 
		CASE(ContinuationEnd);
		CASE(MarkAndSweep);
		CASE(ThisContext);
		CASE(Assign);
		CASE(IfThenReplace);
		CASE(IfThenSuspend);
		CASE(IfThenResume);
		CASE(IfThenReplaceElseResume);
		CASE(IfThenSuspendElseSuspend);
		CASE(IfThenReplaceElseSuspend);
		CASE(IfThenSuspendElseResume);
		CASE(PreInc);
		CASE(PreDec);
		CASE(PostInc);
		CASE(PostDec);
		CASE(ToVector2);
		CASE(ToVector3);
		CASE(ToVector4);
		CASE(LogicalNot); 
		CASE(LogicalAnd); 
		CASE(LogicalOr); 
		CASE(LogicalXor); 
		CASE(LogicalNand);
		CASE(GetProperty);
		CASE(Index);
		CASE(Lookup);

		CASE(If);
		CASE(IfElse);
		CASE(ForEachContained);
		CASE(SuspendNew);

	}
	return "?UNKNOWN_OP?";
}

//EOF
