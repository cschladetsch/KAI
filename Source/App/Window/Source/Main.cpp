#include <iostream>
#include <stdio.h>

#include <KAI/Console/Console.h>
#include <KAI/ImGui/imgui.h>
#include "./imgui_impl_glfw_gl3.h"
#include <stdio.h>
#include <GL/gl3w.h> 
#include <GLFW/glfw3.h>

using namespace std;
using namespace ImGui;

KAI_BEGIN

void ShowExecutorWindow(bool *);

KAI_END

USING_NAMESPACE_KAI


static void error_callback(int error, const char* description)
{
    cerr  << "Error " << error << ": " << description << endl;
}

GLFWwindow *SetupGui()
{
    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
        return 0;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#if KAI_PLATFORM_OSX
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    GLFWwindow* window = glfwCreateWindow(800, 600, "KAI Window", NULL, NULL);
    glfwMakeContextCurrent(window);
    gl3wInit();

    // Setup ImGui binding
    ImGui_ImplGlfwGL3_Init(window, true);

	return window;
}

void LoadFont()
{
    ImGuiIO& io = ImGui::GetIO();
    ImFontConfig config;
    config.OversampleH = 3;
    config.OversampleV = 1;
    config.GlyphExtraSpacing.x = 1.0f;
    io.Fonts->AddFontFromFileTTF("CourierNew.ttf", 20, &config); 
}

int main(int argc, char **argv)
{
	GLFWwindow *window = SetupGui();
	if (!window)
		return -1;

    LoadFont();

    bool show_test_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImColor(114, 144, 154);

    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        ImGui_ImplGlfwGL3_NewFrame();

        // // 1. Show a simple window
        // // Tip: if we don't call ImGui::Begin()/ImGui::End() the widgets appears in a window automatically called "Debug"
        // {
        //     static float f = 0.0f;
        //     ImGui::Text("Hello, world!");
        //     ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
        //     ImGui::ColorEdit3("clear color", (float*)&clear_color);
        //     if (ImGui::Button("Test Window")) show_test_window ^= 1;
        //     if (ImGui::Button("Another Window")) show_another_window ^= 1;
        //     ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        // }

        // // 2. Show another simple window, this time using an explicit Begin/End pair
        // if (show_another_window)
        // {
        //     ImGui::SetNextWindowSize(ImVec2(200,100), ImGuiSetCond_FirstUseEver);
        //     ImGui::Begin("Another Window", &show_another_window);
        //     ImGui::Text("Hello");
        //     ImGui::End();
        // }

        // // 3. Show the ImGui test window. Most of the sample code is in ImGui::ShowTestWindow()
        // if (show_test_window)
        // {
        //     ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiSetCond_FirstUseEver);
        //     ImGui::ShowTestWindow(&show_test_window);
        // }
        
        ShowExecutorWindow(0);

        // Rendering
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui::Render();
        glfwSwapBuffers(window);
    }

    // Cleanup
    ImGui_ImplGlfwGL3_Shutdown();
    glfwTerminate();

    return 0;
}
