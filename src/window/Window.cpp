#include "Window.h"
#include <cstdio>
#include <stdexcept>


static void error_callback (int error, const char* desc) {
	fprintf (stderr, "Error: %s\n", desc);
}


static void key_callback (GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
}


Window::Window () {
	// Init GLFW
	glfwSetErrorCallback (error_callback);
	if (!glfwInit ())
		throw std::runtime_error ("Failed to init GLFW");
	glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create window
	window = glfwCreateWindow (800, 600, "PSO", nullptr, nullptr);
	if (!window)
		throw std::runtime_error ("Failed to create window");
	glfwMakeContextCurrent (window);
	glfwSetKeyCallback (window, key_callback);
	glfwSwapInterval (1);

	// Init GLAD
	gladLoadGLLoader ((GLADloadproc) glfwGetProcAddress);

	// Init Nuklear
	nk_ctx = nk_glfw3_init (window, NK_GLFW3_INSTALL_CALLBACKS);
	nk_font_atlas* font_atlas;
	nk_glfw3_font_stash_begin (&font_atlas);
	nk_glfw3_font_stash_end ();

	// Get functions
	Function::getList (&funcNameList);
	for (size_t i = 0; i < Function::getListSize (); i++)
		functions.push_back (Function::get (std::string(funcNameList[i])));
}


Window::~Window () {
	if (window)
		glfwDestroyWindow (window);
	if (nk_ctx)
		nk_glfw3_shutdown ();
	glfwTerminate ();
	Function::freeList (&funcNameList);
}


void Window::mainLoop () {
	while (!glfwWindowShouldClose (window)) {
		glfwPollEvents ();
		nk_glfw3_new_frame ();
		update ();
		render ();
	}
}


void Window::update () {
	proceedGUI ();
}


void Window::render () {
	glClear (GL_COLOR_BUFFER_BIT);
	int width = 0, height = 0;
	glfwGetWindowSize (window, &width, &height);
	glViewport (0, 0, width, height);
	nk_glfw3_render (NK_ANTI_ALIASING_ON, MAX_VERTEX_BUFFER, MAX_ELEMENT_BUFFER);
	glfwSwapBuffers (window);
}
