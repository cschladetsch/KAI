#include "KAI/Executor/Operation.h"

KAI_BEGIN

const char *Operation::ToString(int value) {
    switch (value) {
#define CASE(N) \
    case N:     \
        return #N;
        CASE(Return)
        CASE(SuspendNew)
        CASE(None)
        CASE(True)
        CASE(False)
        CASE(Equiv)
        CASE(NotEquiv)
        CASE(Less)
        CASE(Greater)
        CASE(LessOrEquiv)
        CASE(GreaterOrEquiv)
        CASE(Suspend)
        CASE(Replace)
        CASE(Resume)
        CASE(CppFunctionCall)
        CASE(CppMethodCall)
        CASE(Plus)
        CASE(Minus)
        CASE(Multiply)
        CASE(Divide)
        CASE(Modulo)
        CASE(TypeOf)
        CASE(Store)
        CASE(Retreive)
        CASE(Remove)
        CASE(New)
        CASE(Assign)
        CASE(ChangeScope)
        CASE(GetChildren)
        CASE(Break)
        CASE(Drop)
        CASE(DropN)
        CASE(Swap)
        CASE(Dup)
        CASE(Rot)
        CASE(RotN)
        CASE(Pick)
        CASE(Clear)
        CASE(Depth)
        CASE(Over)
        CASE(ToArray)
        CASE(ToList)
        CASE(ToMap)
        CASE(ToSet)
        CASE(ToHashMap)
        CASE(ToPair)
        CASE(ToVector2)
        CASE(ToVector3)
        CASE(ToVector4)
        CASE(Expand)
        CASE(Name)
        CASE(Fullname)
        CASE(LogicalNot)
        CASE(LogicalAnd)
        CASE(LogicalOr)
        CASE(LogicalXor)
        CASE(LogicalNand)
        CASE(BitwiseNot)
        CASE(BitwiseAnd)
        CASE(BitwiseOr)
        CASE(BitwiseXor)
        CASE(BitwiseNand)
        CASE(Lookup)
        CASE(TraceAll)
        CASE(Trace)
        CASE(This)
        CASE(Ref)
        CASE(Detach)
        CASE(Delete)
        CASE(Exists)
        CASE(Pin)
        CASE(Unpin)
        CASE(ContinuationBegin)
        CASE(ContinuationEnd)
        CASE(MarkAndSweep)
        CASE(ThisContext)
        CASE(ThisContinuation)
        CASE(If)
        CASE(IfElse)
        CASE(ForEachContained)
        CASE(IfThenSuspend)
        CASE(IfThenReplace)
        CASE(IfThenResume)
        CASE(IfThenSuspendElseSuspend)
        CASE(IfThenReplaceElseSuspend)
        CASE(IfThenResumeElseSuspend)
        CASE(IfThenSuspendElseReplace)
        CASE(IfThenReplaceElseReplace)
        CASE(IfThenResumeElseReplace)
        CASE(IfThenSuspendElseResume)
        CASE(IfThenReplaceElseResume)
        CASE(IfThenResumeElseResume)
        CASE(Executor)
        CASE(ExecFile)
        CASE(Assert)
        CASE(NTimes)
        CASE(ForEach)
        CASE(GetProperty)
        CASE(SetProperty)
        CASE(Index)
        CASE(PreInc)
        CASE(PreDec)
        CASE(PostInc)
        CASE(PostDec)
        CASE(WhileLoop)
        CASE(ForLoop)
        CASE(DoLoop)
        CASE(LevelStack)
        CASE(SetChild)
        CASE(GetChild)
        CASE(RemoveChild)
        CASE(SetManaged)
        CASE(PlusEquals)
        CASE(MinusEquals)
        CASE(MulEquals)
        CASE(DivEquals)
        CASE(ModEquals)
        CASE(ToPi)
        CASE(ToRho)
        CASE(StartPiSequence)
        CASE(StartRhoSequence)
        CASE(GarbageCollect)
    }

    return "UnnnamedOp";
}

char const *Operation::ToString() const { return Operation::ToString(value); }

KAI_END
