#include <KAI/ImGui/imgui.h>

#include <KAI/Console.h>
#include <KAI/Core/Exception.h>

#include <vector>
#include <string>

using namespace std;

KAI_BEGIN

// Demonstrating creating a simple console window, with scrolling, filtering, completion and history.
// For the console example, here we are using a more C++ like approach of declaring a class to hold the data and the functions.

struct ExecutorWindow
{
    char InputBuf[256];

    vector<string> Items;
    vector<string> History;

    int HistoryPos;    // -1: new line, 0..History.Size-1 browsing history.
    bool ScrollToBottom;

	Console _console;
	Tree *_tree;
	Executor *_exec;
	Registry *_reg;
	
    ExecutorWindow()
    {
        ClearLog();
        memset(InputBuf, 0, sizeof(InputBuf));
        HistoryPos = -1;

		_console.SetLanguage(Language::Pi);
		_exec = &*_console.GetExecutor();
		_reg = &_console.GetRegistry();
		_tree = &_console.GetTree();
    }

    void ClearLog()
    {
        Items.clear();
        ScrollToBottom = true;
    }

    void AddLog(const char* fmt, ...) 
    {
        char buf[1024];
        va_list args;
        va_start(args, fmt);
        vsnprintf(buf, sizeof(buf), fmt, args);
		buf[strlen(buf)] = 0;
        va_end(args);

        Items.push_back(buf);
        ScrollToBottom = true;
    }

    void Draw(const char* title, bool* p_open)
    {
        ImGui::SetNextWindowSize(ImVec2(520,600), ImGuiSetCond_FirstUseEver);
        if (!ImGui::Begin(title, p_open))
        {
            ImGui::End();
            return;
        }

        // ImGui::TextWrapped("This example implements a console with basic coloring, completion and history. A more elaborate implementation may want to store entries along with extra data such as timestamp, emitter, etc.");
        // ImGui::TextWrapped("Enter 'HELP' for help, press TAB to use text completion.");

        // TODO: display items starting from the bottom

        // if (ImGui::SmallButton("Add Dummy Text")) { AddLog("%d some text", Items.Size); AddLog("some more text"); AddLog("display very important message here!"); } ImGui::SameLine();
        // if (ImGui::SmallButton("Add Dummy Error")) AddLog("[error] something went wrong"); ImGui::SameLine();
        if (ImGui::SmallButton("Clear")) ClearLog(); 
		
		ImGui::SameLine();

        if (ImGui::SmallButton("Scroll to bottom")) ScrollToBottom = true;
        //static float t = 0.0f; if (ImGui::GetTime() - t > 0.02f) { t = ImGui::GetTime(); AddLog("Spam %f", t); }

        ImGui::Separator();

        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0,0));
        // static ImGuiTextFilter filter;
        // filter.Draw("Filter (\"incl,-excl\") (\"error\")", 180);
        ImGui::PopStyleVar();
        ImGui::Separator();

        ImGui::BeginChild("ScrollingRegion", ImVec2(0,-ImGui::GetItemsLineHeightWithSpacing()), false, ImGuiWindowFlags_HorizontalScrollbar);
        if (ImGui::BeginPopupContextWindow())
        {
            if (ImGui::Selectable("Clear")) ClearLog();
            ImGui::EndPopup();
        }

        // Display every line as a separate entry so we can change their color or add custom widgets. If you only want raw text you can use ImGui::TextUnformatted(log.begin(), log.end());
        // NB- if you have thousands of entries this approach may be too inefficient and may require user-side clipping to only process visible items.
        // You can seek and display only the lines that are visible using the ImGuiListClipper helper, if your elements are evenly spaced and you have cheap random access to the elements.
        // To use the clipper we could replace the 'for (int i = 0; i < Items.Size; i++)' loop with:
        //     ImGuiListClipper clipper(Items.Size);
        //     while (clipper.Step())
        //         for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++)
        // However take note that you can not use this code as is if a filter is active because it breaks the 'cheap random-access' property. We would need random-access on the post-filtered list.
        // A typical application wanting coarse clipping and filtering may want to pre-compute an array of indices that passed the filtering test, recomputing this array when user changes the filter,
        // and appending newly elements as they are inserted. This is left as a task to the user until we can manage to improve this example code!
        // If your items are of variable size you may want to implement code similar to what ImGuiListClipper does. Or split your data into fixed height items to allow random-seeking into your list.
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4,1)); // Tighten spacing
        for (size_t i = 0; i < Items.size(); i++)
        {
            const string &item = Items[i];
            // if (!filter.PassFilter(item))
            //     continue;
            ImVec4 col = ImVec4(1.0f,1.0f,1.0f,1.0f); // A better implementation may store a type per-item. For the sample let's just parse the text.
            // if (strstr(item, "[error]")) col = ImColor(1.0f,0.4f,0.4f,1.0f);
            // else if (strncmp(item, "# ", 2) == 0) col = ImColor(1.0f,0.78f,0.58f,1.0f);
            // ImGui::PushStyleColor(ImGuiCol_Text, col);
            ImGui::TextUnformatted(item.c_str());
            // ImGui::PopStyleColor();
        }
        if (ScrollToBottom)
            ImGui::SetScrollHere();
        ScrollToBottom = false;
        ImGui::PopStyleVar();
        ImGui::EndChild();
        ImGui::Separator();

        // Command-line
        if (ImGui::InputText("Input", InputBuf, sizeof(InputBuf), 
			ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CallbackCompletion | ImGuiInputTextFlags_CallbackHistory, &TextEditCallbackStub, (void*)this))
        {
            char* input_end = InputBuf+strlen(InputBuf);
            while (input_end > InputBuf && input_end[-1] == ' ') input_end--; *input_end = 0;
            if (InputBuf[0])
                ExecCommand(InputBuf);
            strcpy(InputBuf, "");
        }

        // Demonstrate keeping auto focus on the input box
        if (ImGui::IsItemHovered() || (ImGui::IsRootWindowOrAnyChildFocused() && !ImGui::IsAnyItemActive() && !ImGui::IsMouseClicked(0)))
            ImGui::SetKeyboardFocusHere(-1); // Auto focus previous widget

        ImGui::End();
    }

    void ExecCommand(const char* command_line)
    {
		try
		{
			_console.Execute(command_line, Structure::Expression);
		}
		catch (Exception::Base &e)
		{
			StringStream st;
			st << e.ToString() << "\n";
			AddLog(st.ToString().c_str());
			return;
		}

		ClearLog();

		for (auto obj : *_exec->GetDataStack())
		{
			StringStream st;
			st << obj << "\n";
			AddLog(st.ToString().c_str());
		}

        // Insert into history. First find match and delete it so it can be pushed to the back. This isn't trying to be smart or optimal.
        HistoryPos = -1;
        for (int i = (int)History.size() - 1; i >= 0; i--)
		{
            // if (Stricmp(History[i], command_line) == 0)
            // {
            //     free(History[i]);
            //     History.erase(History.begin() + i);
            //     break;
            // }
		}

        History.push_back(command_line);

        // Process command
        // AddLog("typed: '%s'\n", command_line);
    }

    static int TextEditCallbackStub(ImGuiTextEditCallbackData* data)
    {
        ExecutorWindow* console = (ExecutorWindow*)data->UserData;
        return console->TextEditCallback(data);
    }

    int TextEditCallback(ImGuiTextEditCallbackData* data)
    {
        // //AddLog("cursor: %d, selection: %d-%d", data->CursorPos, data->SelectionStart, data->SelectionEnd);
        // switch (data->EventFlag)
        // {
        // case ImGuiInputTextFlags_CallbackCompletion:
        //     {
        //         // Example of TEXT COMPLETION

        //         // Locate beginning of current word
        //         const char* word_end = data->Buf + data->CursorPos;
        //         const char* word_start = word_end;
        //         while (word_start > data->Buf)
        //         {
        //             const char c = word_start[-1];
        //             if (c == ' ' || c == '\t' || c == ',' || c == ';')
        //                 break;
        //             word_start--;
        //         }

        //         // Build a list of candidates
        //         ImVector<const char*> candidates;
        //         for (int i = 0; i < Commands.Size; i++)
        //             if (Strnicmp(Commands[i], word_start, (int)(word_end-word_start)) == 0)
        //                 candidates.push_back(Commands[i]);

        //         if (candidates.Size == 0)
        //         {
        //             // No match
        //             AddLog("No match for \"%.*s\"!\n", (int)(word_end-word_start), word_start);
        //         }
        //         else if (candidates.Size == 1)
        //         {
        //             // Single match. Delete the beginning of the word and replace it entirely so we've got nice casing
        //             data->DeleteChars((int)(word_start-data->Buf), (int)(word_end-word_start));
        //             data->InsertChars(data->CursorPos, candidates[0]);
        //             data->InsertChars(data->CursorPos, " ");
        //         }
        //         else
        //         {
        //             // Multiple matches. Complete as much as we can, so inputing "C" will complete to "CL" and display "CLEAR" and "CLASSIFY"
        //             int match_len = (int)(word_end - word_start);
        //             for (;;)
        //             {
        //                 int c = 0;
        //                 bool all_candidates_matches = true;
        //                 for (int i = 0; i < candidates.Size && all_candidates_matches; i++)
        //                     if (i == 0)
        //                         c = toupper(candidates[i][match_len]);
        //                     else if (c == 0 || c != toupper(candidates[i][match_len]))
        //                         all_candidates_matches = false;
        //                 if (!all_candidates_matches)
        //                     break;
        //                 match_len++;
        //             }

        //             if (match_len > 0)
        //             {
        //                 data->DeleteChars((int)(word_start - data->Buf), (int)(word_end-word_start));
        //                 data->InsertChars(data->CursorPos, candidates[0], candidates[0] + match_len);
        //             }

        //             // List matches
        //             AddLog("Possible matches:\n");
        //             for (int i = 0; i < candidates.Size; i++)
        //                 AddLog("- %s\n", candidates[i]);
        //         }

        //         break;
        //     }
        // case ImGuiInputTextFlags_CallbackHistory:
        //     {
        //         // Example of HISTORY
        //         const int prev_history_pos = HistoryPos;
        //         if (data->EventKey == ImGuiKey_UpArrow)
        //         {
        //             if (HistoryPos == -1)
        //                 HistoryPos = History.Size - 1;
        //             else if (HistoryPos > 0)
        //                 HistoryPos--;
        //         }
        //         else if (data->EventKey == ImGuiKey_DownArrow)
        //         {
        //             if (HistoryPos != -1)
        //                 if (++HistoryPos >= History.Size)
        //                     HistoryPos = -1;
        //         }

        //         // A better implementation would preserve the data on the current input line along with cursor position.
        //         if (prev_history_pos != HistoryPos)
        //         {
        //             data->CursorPos = data->SelectionStart = data->SelectionEnd = data->BufTextLen = (int)snprintf(data->Buf, (size_t)data->BufSize, "%s", (HistoryPos >= 0) ? History[HistoryPos] : "");
        //             data->BufDirty = true;
        //         }
        //     }
        // }
        return 0;
    }
};

void ShowExecutorWindow(bool* p_open)
{
    static ExecutorWindow console;
    console.Draw("KAI Console", p_open);
}

KAI_END

