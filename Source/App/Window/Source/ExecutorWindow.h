#pragma once

#include <KAI/ImGui/imgui.h>

#include <KAI/Console.h>
#include <KAI/Core/Exception.h>

#include <vector>
#include <string>

using namespace std;

KAI_BEGIN

// TODO: it's pretty old-school to not have a header and just use a single
// static instance via ShowExecutorWindow()....
struct ExecutorWindow
{
protected:
	// deprecating...
    char InputBuf[256];
    int HistoryPos;    // -1: new line, 0..History.Size-1 browsing history.
    bool ScrollToBottom;

    vector<string> Items;
    vector<string> History;

	Console _console;
	Tree *_tree;
	Executor *_exec;
	Registry *_reg;
	
public:
    ExecutorWindow();

    void ClearLog();
    void AddLog(const char* fmt, ...);
    void Draw(const char* title, bool* p_open);
    void ExecCommand(const char* command_line);
    static int TextEditCallbackStub(ImGuiTextEditCallbackData* data);
    int TextEditCallback(ImGuiTextEditCallbackData* data);
};

void ShowExecutorWindow(bool* p_open);

KAI_END

