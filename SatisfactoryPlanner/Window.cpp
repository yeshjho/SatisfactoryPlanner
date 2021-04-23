#include "Window.h"

#include <iostream>
#include <map>
#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>



bool Window::ShouldClose() const
{
	return glfwWindowShouldClose(mpWindow);
}


void Window::SwapBuffers() const
{
	glfwSwapBuffers(mpWindow);
}



void gl_debug_callback(const GLenum source, const GLenum type, const GLuint id, const GLenum severity, [[maybe_unused]] GLsizei length, const GLchar* message, [[maybe_unused]] const void* userData)
{
	static const std::map<GLenum, std::string> sources{
		{ GL_DEBUG_SOURCE_API, "API" },
		{ GL_DEBUG_SOURCE_APPLICATION, "APPLICATION" },
		{ GL_DEBUG_SOURCE_SHADER_COMPILER, "SHADER" },
		{ GL_DEBUG_SOURCE_THIRD_PARTY, "THIRD_PARTY" },
		{ GL_DEBUG_SOURCE_WINDOW_SYSTEM, "WINDOW_SYSTEM" },
		{ GL_DEBUG_SOURCE_OTHER, "OTHER" },
	};

	static const std::map<GLenum, std::string> types{
		{ GL_DEBUG_TYPE_ERROR, "ERROR" },
		{ GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR, "DEPRECATED" },
		{ GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR, "UNDEFINED" },
		{ GL_DEBUG_TYPE_PORTABILITY, "PORTABILITY" },
		{ GL_DEBUG_TYPE_PERFORMANCE, "PERFORMANCE" },
		{ GL_DEBUG_TYPE_OTHER, "OTHER" },
		{ GL_DEBUG_TYPE_MARKER, "MARKER" },
	};

	static const std::map<GLenum, std::string> severities{
		{ GL_DEBUG_SEVERITY_NOTIFICATION, "NOTIFICATION" },
		{ GL_DEBUG_SEVERITY_HIGH, "HIGH" },
		{ GL_DEBUG_SEVERITY_MEDIUM, "MEDIUM" },
		{ GL_DEBUG_SEVERITY_LOW, "LOW" },
	};

	std::cout << "Debug Message in OpenGL: ";
	std::cout << (sources.count(source) ? sources.at(source) : "UNKNOWN(" + std::to_string(source) + ")") << "\t|\t";
	std::cout << (types.count(type) ? types.at(type) : "UNKNOWN(" + std::to_string(type) + ")") << "\t|\t";
	std::cout << id << "\t|\t";
	std::cout << (severities.count(severity) ? severities.at(severity) : "UNKNOWN(" + std::to_string(severity) + ")") << "\t|\t";
	std::cout << message << std::endl;
}



Window create_window()
{
	Window window;

	if (!glfwInit())
	{
		std::cerr << "glfw init failed" << std::endl;
		std::exit(-1);
	}
	std::atexit([]() { glfwTerminate(); });

	glfwSetErrorCallback([](int error, const char* desc) { std::cerr << "Error in glfw(" << std::to_string(error) << "): " << desc << std::endl; });

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
	glfwWindowHint(GLFW_RED_BITS, 8);
	glfwWindowHint(GLFW_GREEN_BITS, 8);
	glfwWindowHint(GLFW_BLUE_BITS, 8);
	glfwWindowHint(GLFW_ALPHA_BITS, 8);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
#ifdef _DEBUG
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
#endif

	window.SetWindow(glfwCreateWindow(1366, 768, "titleeee", nullptr, nullptr));
	if (!window.GetWindow())
	{
		std::cerr << "glfw create window failed" << std::endl;
		std::exit(-1);
	}

	glfwMakeContextCurrent(window.GetWindow());

	glfwSetFramebufferSizeCallback(window.GetWindow(),
		[](GLFWwindow*, const int width, const int height)
		{
			glViewport(0, 0, width, height);
		});
	/*


		glfwSetCursorPosCallback(p_window, [](GLFWwindow*, double x, double y) { if (mouse_moved_callback) mouse_moved_callback(x, y); });
		glfwSetMouseButtonCallback(p_window, [](GLFWwindow*, int button, int action, int mod) { if (mouse_button_callback) mouse_button_callback(button, action, mod); });
		glfwSetScrollCallback(p_window, [](GLFWwindow*, double xAmount, double yAmount) { if (mouse_wheel_callback) mouse_wheel_callback(xAmount, yAmount); });
		glfwSetKeyCallback(p_window, [](GLFWwindow*, int key, int scanCode, int action, int mod) { if (key_button_callback) key_button_callback(key, scanCode, action, mod); });

		glfwSetInputMode(p_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	 */

	if (glewInit() != GLEW_OK)
	{
		std::cerr << "glew init failed" << std::endl;
		std::exit(-1);
	}

	if (!GLEW_VERSION_4_5)
	{
		std::cerr << "driver doesn't support 4.5" << std::endl;
		std::exit(-1);
	}

	/*
	 * 		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_DEPTH_TEST);
	 */

#ifdef _DEBUG
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(gl_debug_callback, nullptr);
#endif

	return window;
}
