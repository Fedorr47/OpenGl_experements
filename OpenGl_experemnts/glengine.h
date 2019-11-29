#pragma once
#include <memory>
#include <functional>
#include <iostream>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// Other Libs
#include <SOIL.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class GlEngineException : public std::exception
{
private:
	std::string what_;
	int error_code_{ 0 };
public:
	GlEngineException(
		std::string error_str,
		int error_code
	) :
		what_(error_str),
		error_code_(error_code)
	{}
	virtual const char* what() const throw()
	{
		return what_.c_str();
	}
};
class GlEngine
{
	GLFWkeyfun			extern_key_function;
	GLFWcursorposfun	extern_curpos_function;
	GLFWscrollfun		extern_scroll_function;
public:
	GlEngine() { InitGL(); };
	~GlEngine();
	void InitGL();
	GLFWwindow* get_window();
	enum callback_type
	{
		KEY,
		MOUSE,
		SCROLL
	};
	template<typename Callable>
	void add_callback(
		callback_type type,
		Callable&& callbackFunc
	);
	
private:
	enum ErrorCodes {
		NO_INIT
	};
	GLFWwindow* window_;
	bool is_init_{ false };
	const GLuint screenWidth = 800;
	const GLuint screenHeight = 600;
	std::string window_name = "OpenGLwindow";
};

GlEngine::~GlEngine()
{
	glfwTerminate();
}

void GlEngine::InitGL()
{
	// glfw
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_SAMPLES, 4);
	window_ = 
		glfwCreateWindow(
			screenWidth,
			screenHeight,
			window_name.c_str(),
			nullptr,
			nullptr);

	glfwMakeContextCurrent(window_);

	// Options
	glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	// Initialize GLEW to setup the OpenGL Function pointers
	glewExperimental = GL_TRUE;
	glewInit();
	// Define the viewport dimensions
	glViewport(0, 0, screenWidth, screenHeight);
	// Setup some OpenGL options
	glEnable(GL_DEPTH_TEST);

	is_init_ = true;
}

GLFWwindow* GlEngine::get_window()
{
	return window_;
}



template<typename Callable>
inline void GlEngine::add_callback(
	callback_type type,
	Callable&& callbackFunc
)
{	
	switch (type)
	{
	case callback_type::KEY:
		extern_key_function = GLFWkeyfun(callbackFunc);
		glfwSetKeyCallback(window_, extern_key_function);
		break;
	case callback_type::MOUSE:
		extern_curpos_function = GLFWcursorposfun(callbackFunc);
		glfwSetCursorPosCallback(window_, extern_curpos_function);
		break;
	case callback_type::SCROLL:
		extern_scroll_function = GLFWscrollfun(callbackFunc);
		glfwSetScrollCallback(window_, extern_scroll_function);
		break;
	}
}

std::unique_ptr<GlEngine> GlEngine_ptr;

