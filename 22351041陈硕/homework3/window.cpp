#include "window.h"

//std
#include <stdexcept>

Window::Window(int w, int h, std::string name) : width(w), height(h), windowName(name)
{
	initWindow();
}

Window::~Window()
{
	glfwDestroyWindow(window);
	glfwTerminate();
}

void Window::frambufferResizeCallback(GLFWwindow* window, int width, int height)
{
	auto lvewindow = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
	lvewindow->frambufferResized = true;
	lvewindow->width = width;
	lvewindow->height = height;
	glViewport(0, 0, width, height);
}

void Window::initWindow()
{
	//��ʼ��
	glfwInit();
	//�������汾�źʹ˰汾��
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//����profile
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//��������
	window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
	//����Ϊ��Ҫ��������
	glfwMakeContextCurrent(window);
	//set to be the parent window object
	glfwSetWindowUserPointer(window, this);
	// tell GLFW to capture our mouse
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	//���贰�ڴ�С
	glfwSetFramebufferSizeCallback(window, frambufferResizeCallback);
}
